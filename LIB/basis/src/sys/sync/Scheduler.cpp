#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/thread.hpp>
#include <basis/simstd/algorithm>
#include <basis/simstd/mutex>
#include <basis/simstd/vector>

namespace sync
{
	LogRegisterLocal(L"sync");

	struct InterestMapping
	{
		InterestMapping(EventHandler* callback, MessageI::param_type type, MessageI::param_type mask):
			callback(callback),
			type(type),
			mask(mask)
		{
			CRT_ASSERT(callback);
		}

		bool check(const Message& message) const
		{
			return (check_mask(message) && check_filter(message)) ? true : false;
		}

		bool operator ()(const Message& message) const
		{
			callback->handle_event(message);
			return true;
		}

		bool operator ==(EventHandler* callback) const
		{
			return this->callback == callback;
		}

	private:
		bool check_mask(const Message& message) const
		{
			return (type & mask) && (message->get_type() & mask);
		}

		bool check_filter(const Message& /*message*/) const
		{
//			return !filter || filter(message);
			return true;
		}

		EventHandler* callback;
		MessageI::param_type type;
		MessageI::param_type mask;

		friend bool operator <(const InterestMapping& mapping, EventHandler* callback);
		friend bool operator <(EventHandler* callback, const InterestMapping& mapping);
	};

	inline bool operator <(const InterestMapping& mapping, EventHandler* callback)
	{
		return mapping.callback < callback;
	}

	inline bool operator <(EventHandler* callback, const InterestMapping& mapping)
	{
		return callback < mapping.callback;
	}

	using TaskPlan = simstd::pair<int64_t, Task*>;
	bool operator ==(const TaskPlan& a, Task* b)
	{
		return a.second == b;
	}

	struct TaskPlanGreater
	{
		bool operator ()(const TaskPlan& a, const TaskPlan& b)
		{
			return b.first < a.first;
		}
		bool operator ()(int64_t a, const TaskPlan& b)
		{
			return b.first < a;
		}
		bool operator ()(const TaskPlan& a, int64_t b)
		{
			return b < a.first;
		}
	};


	class Scheduler::TasksQueueImpl: public CriticalSection, private simstd::vector<TaskPlan>
	{
	public:
		int64_t get_activate_time() const
		{
			auto guard(simstd::auto_lock(*this));
			return empty() ? WAIT_FOREVER : back().first;
		}

		void add_task(Task* task, int64_t timeout)
		{
			auto guard(simstd::auto_lock(*this));
			LogTraceObj(L"(%p, %I64d)\n", task, timeout);
			auto nowTime = sync::now();
			auto activateTime = nowTime + timeout;
			LogTraceObj(L"nowTime: %I64d, activateTime: %I64d\n", nowTime, activateTime);
			emplace(simstd::upper_bound(begin(), end(), activateTime, TaskPlanGreater()), activateTime, task);
		}

		bool del_task(Task* task)
		{
			auto guard(simstd::auto_lock(*this));
			auto it = simstd::find(begin(), end(), task);
			return (it != end()) ? erase(it), true : false;
		}

		Task* get_task()
		{
			auto guard(simstd::auto_lock(*this));
			auto ret = back();
			pop_back();
			return ret.second;
		}

		bool is_exist(Task* task) const
		{
			auto guard(simstd::auto_lock(*this));
			return simstd::find(begin(), end(), task) != end();
		}
	};

	class Scheduler::InterQueueImpl: public CriticalSection, private simstd::vector<InterestMapping>
	{
		using base_type = simstd::vector<InterestMapping>;

	public:
		void add_mapping(EventHandler* callback, MessageI::param_type type, MessageI::param_type mask)
		{
			auto guard(simstd::auto_lock(*this));
			emplace(simstd::upper_bound(begin(), end(), callback), callback, type, mask);
		}

		void del_mapping(EventHandler* callback)
		{
			auto guard(simstd::auto_lock(*this));
			auto range = simstd::equal_range(begin(), end(), callback);
			erase(range.first, range.second);
		}

		void notify(const Message& message) const
		{
			base_type tmp;
			{
				auto guard(simstd::auto_lock(*this));
				for (const InterestMapping& item : *this) {
					if (item.check(message))
						tmp.emplace_back(item);
				}
			}
			for (const InterestMapping& item : tmp) {
				item(message);
			}
		}
	};

	class Scheduler::ThreadImpl: private thread::Routine, public thread::Unit
	{
	public:
		ThreadImpl(Scheduler* scheduler, ssize_t stackSize):
			Unit(this, true, stackSize),
			scheduler(scheduler)
		{
			LogTraceObjLn();
		}

		ssize_t run(void* data) override
		{
			LogTraceObjLn();
			return scheduler->run(data);
		}

	private:
		Scheduler* scheduler;
	};

	Scheduler::~Scheduler()
	{
		LogTraceObjLn();
		event_queue->put_message(create_message(MessageI::SCHEDULER, MessageI::SCHEDULER_EXIT));
		thread->wait(WAIT_FOREVER);
	}

	Scheduler::Scheduler(const wchar_t* name, ssize_t stackSize):
		event_queue(),
		tasks_queue(),
		inter_queue(),
		thread()
	{
		LogTraceObj(L"begin ('%s', %Id)\n", name, stackSize);
		event_queue = create_queue(name);
		tasks_queue = simstd::make_unique<TasksQueueImpl>();
		inter_queue = simstd::make_unique<InterQueueImpl>();
		thread = simstd::make_unique<ThreadImpl>(this, stackSize);
		LogTraceObj(L"end\n");

		thread->resume();
	}

	bool Scheduler::arrange_task(Task* task, int64_t timeout)
	{
		LogTraceObj(L"(%p, %I64d)\n", task, timeout);
//		if (_base->is_destroying)
//			return is_task_scheduled;
//		const int64_t currentClock = _base->timer.result_ms();
//		const int64_t launchTime = timeout != INFIN ? currentClock + timeout : timeout;

		{
			auto guard(simstd::auto_lock(*tasks_queue));
			const bool is_task_scheduled = tasks_queue->is_exist(task);
			if (is_task_scheduled)
				return true;
		}
		tasks_queue->add_task(task, timeout);
		event_queue->put_message(create_message(MessageI::SCHEDULER, MessageI::SCHEDULER_UPDATE));
		return true;
	}

	bool Scheduler::cancel_task(Task* task)
	{
		LogTraceLn();
		return tasks_queue->del_task(task);
	}

	bool Scheduler::is_scheduled(Task* task) const
	{
		LogTraceLn();
		return tasks_queue->is_exist(task);
	}

	Queue Scheduler::get_queue() const
	{
		LogTraceLn();
		return event_queue;
	}

	void Scheduler::register_interest(EventHandler* callback, MessageI::param_type type, MessageI::param_type mask, bool /*is_broadcast_event*/)
	{
		LogTraceObjLn();
		CRT_ASSERT(callback);
//		if (_base->is_destroying)
//			return is_task_scheduled;
		inter_queue->add_mapping(callback, type, mask);
	}

	void Scheduler::unregister_interest(EventHandler* callback)
	{
		LogTraceObjLn();
		CRT_ASSERT(callback);
		inter_queue->del_mapping(callback);
	}

	void Scheduler::put_message(const Message& message)
	{
		event_queue->put_message(message);
	}

	ssize_t Scheduler::run(void* data)
	{
		UNUSED(data);

		LogTraceObjLn();
		while (true) {
			auto activateClock = tasks_queue->get_activate_time();
//			LogTraceObj(L"activateClock: %I64d\n", activateClock);
			auto activateTimeout = (activateClock == int64_t(WAIT_FOREVER)) ? int64_t(WAIT_FOREVER) : activateClock - simstd::min(activateClock, sync::now());
//			LogTraceObj(L"activateTimeout: %I64d\n", activateTimeout);

			Message msg;
			auto reason = event_queue->get_message(msg, activateTimeout);
			if (reason == WaitResult_t::SUCCESS) {
				if (msg->get_type() == MessageI::SCHEDULER) {
					if (msg->get_a() == MessageI::SCHEDULER_EXIT) {
						LogDebug2(L"exit message received\n");
						break;
					}

					if (msg->get_a() == MessageI::SCHEDULER_UPDATE) {
						LogDebug2(L"update message received\n");
						continue;
					}
				} else {
					LogDebug2(L"send round message to subscribers\n");
					inter_queue->notify(msg);
				}
			} else if (reason == WaitResult_t::TIMEOUT) {
				LogDebug2(L"timeout\n");
				fire_tasks();
			} else {
				break;
			}
		}

		LogDebug(L"finishing thread\n");
		return 0;
	}

	void Scheduler::fire_tasks()
	{
		LogTraceObjLn();
		auto currentTime = sync::now();
		while (tasks_queue->get_activate_time() <= currentTime) {
			tasks_queue->get_task()->execute(false);
		}
	}
}
