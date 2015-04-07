#ifndef BASIS_SYS_SYNC_SCHEDULER_HPP_
#define BASIS_SYS_SYNC_SCHEDULER_HPP_

namespace sync
{
	class Task
	{
	public:
		bool execute(bool exiting = false) {return do_execute(exiting);}

		virtual ~Task() = default;

		virtual const wchar_t* get_name() const = 0;

	private:
		virtual bool do_execute(bool exiting) = 0;
	};

	struct EventHandler
	{
		virtual ~EventHandler() = default;
		virtual bool handle_event(const Message& message) = 0;
	};

	class Scheduler: private CriticalSection
	{
	public:
		~Scheduler();

		Scheduler(const wchar_t* name, ssize_t stackSize = 0);

		bool arrange_task(Task* task, int64_t timeout);

		bool cancel_task(Task* task);

		bool is_scheduled(Task* task) const;

		Queue get_queue() const;

		void register_interest(EventHandler* callback, MessageI::param_type type, MessageI::param_type mask, bool is_broadcast_event = false);

		void unregister_interest(EventHandler* callback);

		void put_message(const Message& message);

	private:
		Scheduler();

		ssize_t run(void* data);

		void fire_tasks();

		class TasksQueueImpl;
		class InterQueueImpl;
		class ThreadImpl;
		using TasksQueue = simstd::unique_ptr<TasksQueueImpl>;
		using InterQueue = simstd::unique_ptr<InterQueueImpl>;
		using Thread = simstd::unique_ptr<ThreadImpl>;
		using EventQueue = Queue;

		EventQueue event_queue;
		TasksQueue tasks_queue;
		InterQueue inter_queue;
		Thread thread;
	};
}

#endif
