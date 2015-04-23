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
		~InterestMapping()
		{
			if (subId)
				delivery::unsubscribe(subId);
		}

		InterestMapping(EventHandler* callback, message::param_type type, message::param_type mask, delivery::SubscribtionId subId):
			callback(callback),
			type(type),
			mask(mask),
			subId(subId)
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
		message::param_type type;
		message::param_type mask;
		delivery::SubscribtionId subId;

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

	class Scheduler::TasksQueueImpl: public CriticalSection
	{
		using base_type = simstd::vector<InterestMapping>;
	public:
		int64_t get_activate_time() const;
		bool add_task(Task* task, int64_t activateTime);
		bool del_task(Task* task);
		bool is_exist(Task* task) const;
		bool get_task(Task*& task, int64_t currentTime);

		void add_mapping(EventHandler* callback, message::param_type type, message::param_type mask, delivery::SubscribtionId subId);
		void del_mapping(EventHandler* callback);
		void notify(const Message& message) const;

	private:
		simstd::vector<TaskPlan> plan;
		simstd::vector<InterestMapping> interests;
	};

	class Scheduler::ThreadImpl: private thread::Routine, public thread::Unit
	{
	public:
		ThreadImpl(SchedulerImplStruct* impl, ssize_t stackSize);
		ssize_t run(void* data) override;

	private:
		SchedulerImplStruct* impl;
	};

	struct Scheduler::SchedulerImplStruct
	{
		Queue messg_queue;
		Scheduler::TasksQueueImpl tasks_queue;
		Scheduler::ThreadImpl thread;
		Task* currentTask = nullptr;

		SchedulerImplStruct(const wchar_t* name, ssize_t stackSize);

		bool cancel_task(Task* task);

		ssize_t run(void* data);

		void fire_tasks();
	};

	///=================================================================================================================
	int64_t Scheduler::TasksQueueImpl::get_activate_time() const
	{
		auto guard(simstd::auto_lock(*this));
		return plan.empty() ? TIMEOUT_INFINITE : plan.back().first;
	}

	bool Scheduler::TasksQueueImpl::add_task(Task* task, int64_t activateTime)
	{
		auto guard(simstd::auto_lock(*this));
		LogTraceObj(L"(%p, %I64d)\n", task, activateTime);
		auto position = simstd::upper_bound(plan.cbegin(), plan.cend(), activateTime, TaskPlanGreater());
		auto ret = position == plan.cend();
		plan.emplace(position, activateTime, task);
		return ret;
	}

	bool Scheduler::TasksQueueImpl::del_task(Task* task)
	{
		auto guard(simstd::auto_lock(*this));
		auto it = simstd::find(plan.cbegin(), plan.cend(), task);
		return (it != plan.cend()) ? plan.erase(it), true : false;
	}

	bool Scheduler::TasksQueueImpl::get_task(Task*& task, int64_t currentTime)
	{
		bool ret = false;
		auto guard(simstd::auto_lock(*this));
		if (!plan.empty() && plan.back().first <= currentTime) {
			task = plan.back().second;
			plan.pop_back();
			ret = true;
		}
		return ret;
	}

	bool Scheduler::TasksQueueImpl::is_exist(Task* task) const
	{
		auto guard(simstd::auto_lock(*this));
		return simstd::find(plan.cbegin(), plan.cend(), task) != plan.cend();
	}

	void Scheduler::TasksQueueImpl::add_mapping(EventHandler* callback, message::param_type type, message::param_type mask, delivery::SubscribtionId subId)
	{
		auto guard(simstd::auto_lock(*this));
		interests.emplace(simstd::upper_bound(interests.cbegin(), interests.cend(), callback), callback, type, mask, subId);
	}

	void Scheduler::TasksQueueImpl::del_mapping(EventHandler* callback)
	{
		auto guard(simstd::auto_lock(*this));
		auto range = simstd::equal_range(interests.cbegin(), interests.cend(), callback);
		if (range.first != range.second) {
			interests.erase(range.first, range.second);
		}
	}

	void Scheduler::TasksQueueImpl::notify(const Message& message) const
	{
		LogDebug2(L"send round message to subscribers\n");
		base_type tmp;
		{
			auto guard(simstd::auto_lock(*this));
			for (const InterestMapping& item : interests) {
				if (item.check(message))
					tmp.emplace_back(item);
			}
		}
		for (const InterestMapping& item : tmp) {
			item(message);
		}
	}

	///=================================================================================================================
	Scheduler::ThreadImpl::ThreadImpl(SchedulerImplStruct* impl, ssize_t stackSize):
		Unit(this, true, stackSize),
		impl(impl)
	{
		LogTraceObjLn();
	}

	ssize_t Scheduler::ThreadImpl::run(void* data)
	{
		LogTraceObjLn();
		return impl->run(data);
	}

	///=================================================================================================================
	Scheduler::SchedulerImplStruct::SchedulerImplStruct(const wchar_t* name, ssize_t stackSize):
		messg_queue(create_queue(name)),
		tasks_queue(),
		thread(this, stackSize)
	{
	}

	bool Scheduler::SchedulerImplStruct::cancel_task(Task* task)
	{
		LogTraceObj(L"(%p)\n", task);
		auto ret = tasks_queue.del_task(task);
		if (!ret && task == currentTask) {
			LogDebug(L"we are trying to cancel task already running: %p\n", task);
			if (thread::get_id() != thread.get_id()) {
				LogAtten(L"waiting till task is finished: %p\n", task);
				while (task == currentTask)
					sync::sleep(0);
			} else {
				LogInfo(L"cancelling task from itself: %p\n", task);
			}
		}
		LogTraceObj(L"(%p) -> %d\n", task, ret);
		return ret;
	}

	ssize_t Scheduler::SchedulerImplStruct::run(void* data)
	{
		UNUSED(data);

		LogTraceObjLn();
		while (true) {
			auto activateClock = tasks_queue.get_activate_time();
//			LogTraceObj(L"activateClock: %I64d\n", activateClock);
			auto activateTimeout = (activateClock == TIMEOUT_INFINITE) ? TIMEOUT_INFINITE : activateClock - simstd::min(activateClock, sync::now());
//			LogTraceObj(L"activateTimeout: %I64d\n", activateTimeout);

			Message msg;
			auto reason = messg_queue->get_message(msg, activateTimeout);
			if (reason == WaitResult_t::SUCCESS) {
				if (msg->get_type() == message::SYSTEM) {
					if (msg->get_a() == message::SYSTEM_SCHEDULER_STOP) {
						break;
					}
					if (msg->get_a() == message::SYSTEM_SCHEDULER_UPDATE) {
						LogDebug2(L"update message received\n");
						fire_tasks();
						continue;
					}
				} else {
					tasks_queue.notify(msg);
				}
			} else if (reason == WaitResult_t::TIMEOUT) {
				fire_tasks();
			} else {
				break;
			}
		}

		LogInfo(L"finishing thread\n");
		return 0;
	}

	void Scheduler::SchedulerImplStruct::fire_tasks()
	{
		LogTraceObjLn();
		while (tasks_queue.get_task(currentTask, sync::now())) {
			LogDebug(L"task starting: %p, '%s'\n", currentTask, currentTask->get_name());
			currentTask->execute(false);
			LogDebug(L"task ended: %p\n", currentTask);
			currentTask = nullptr;
		}
	}

	///=================================================================================================================
	Scheduler::~Scheduler()
	{
		LogTraceObjLn();
		destroying = true;
		impl->messg_queue->put_message(message::create(message::SYSTEM, message::SYSTEM_SCHEDULER_STOP));
		impl->thread.wait();
	}

	Scheduler::Scheduler(const wchar_t* name, ssize_t stackSize)
	{
		LogTraceObj(L"begin ('%s', %Id)\n", name, stackSize);
		impl = simstd::make_unique<SchedulerImplStruct>(name, stackSize);
		LogTraceObj(L"end\n");

		impl->thread.resume();
	}

	bool Scheduler::arrange_task(Task* task, int64_t timeout)
	{
		LogTraceObj(L"(%p, %I64d)\n", task, timeout);
		if (destroying)
			return false;

		if (impl->tasks_queue.is_exist(task))
			return true;

		auto nowTime = sync::now();
		auto activateTime = nowTime + timeout;
		LogTraceObj(L"nowTime: %I64d, activateTime: %I64d\n", nowTime, activateTime);

		if (impl->tasks_queue.add_task(task, activateTime)) {
			LogDebug(L"task is first in queue, updating wait time\n");
			impl->messg_queue->put_message(message::create(message::SYSTEM, message::SYSTEM_SCHEDULER_UPDATE));
		}
		return true;
	}

	bool Scheduler::cancel_task(Task* task)
	{
		LogTraceObj(L"(%p)\n", task);
		return impl->cancel_task(task);
	}

	bool Scheduler::is_scheduled(Task* task) const
	{
		LogTraceObjLn();
		return impl->tasks_queue.is_exist(task);
	}

	Queue Scheduler::get_queue() const
	{
		LogTraceObjLn();
		return impl->messg_queue;
	}

	void Scheduler::register_interest(EventHandler* callback, message::param_type type, message::param_type mask, bool broadcastAlso)
	{
		LogTraceObjLn();
		CRT_ASSERT(callback);

		if (destroying)
			return;

		auto subId = (broadcastAlso) ? delivery::subscribe(impl->messg_queue, type, mask) : delivery::SubscribtionId();

		impl->tasks_queue.add_mapping(callback, type, mask, subId);
	}

	void Scheduler::unregister_interest(EventHandler* callback)
	{
		LogTraceObjLn();
		CRT_ASSERT(callback);
		impl->tasks_queue.del_mapping(callback);
	}

	void Scheduler::put_message(const Message& message)
	{
		impl->messg_queue->put_message(message);
	}
}
