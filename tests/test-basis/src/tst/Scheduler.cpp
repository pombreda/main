#include <basis/tst.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/thread.hpp>
#include <basis/simstd/vector>

class MyTask: public sync::Task
{
public:
	~MyTask()
	{
		LogTraceObjLn();
	}

	MyTask(sync::Scheduler* scheduler, const wchar_t* name) :scheduler(scheduler), name(name) {}

	const wchar_t* get_name() const override
	{
		return name;
	}

	bool do_execute(bool exiting) override
	{
		LogTraceObj(L"begin (%d)\n", exiting);
		LogTraceObj(L"sleep\n");
		sync::sleep(30);
		scheduler->cancel_task(this);
		LogTraceObj(L"end and delete\n");
		delete this;
		return true;
	}

	sync::Scheduler* scheduler;
	const wchar_t* name;
};

class MyTimer: public thread::Routine
{
public:
	ssize_t run(void*) override
	{
		while (true) {
			using namespace sync;
			delivery::send_round(create_message(message::APPLICATION, message::APPLICATION_TIMER, now()));
			sleep(100);
		}
		return 0;
	}
};

class Th: public thread::Routine
{
public:
	Th(sync::Scheduler* scheduler): scheduler(scheduler)
	{
	}

	ssize_t run(void*) override
	{
		while (true) {
			simstd::vector<sync::Task*> tasks;
			for (int i = 0; i < 10; ++i)
			{
				tasks.emplace_back(new MyTask(scheduler, L"MyDynTask"));
				LogTrace(L"arranging task: %d, %p\n", i, tasks.back());
				scheduler->arrange_task(tasks.back(), i * 10);
				sync::sleep(3);
			}
			for (int i = 9; i >= 0; --i)
			{
				LogTrace(L"cancelling task: %d, %p\n", i, tasks[i]);
				if (scheduler->cancel_task(tasks[i]))
					delete tasks[i];
				sync::sleep(4);
			}
		}
		return 0;
	}

private:
	sync::Scheduler* scheduler;
};

class MyEventHandler: public sync::EventHandler
{
	bool handle_event(const sync::Message& message) override
	{
		LogTraceObj(L"Message(0x%IX(%Iu, %Iu, %Iu))\n", message->get_type(), message->get_a(), message->get_b(), message->get_c());
		return true;
	}
};

ssize_t tst::_Scheduler()
{
	LogAtten(L"\n");

	sync::Scheduler scheduler(L"tst::_Scheduler");
	thread::Unit th1(new Th(&scheduler));
	thread::Unit th2(new MyTimer);

//	auto myTask1 = new MyTask(&scheduler, L"MyTask1");
//	auto myTask2 = new MyTask(&scheduler, L"MyTask2");
	MyEventHandler myHandler;

	sync::sleep(1000);
//	scheduler.arrange_task(myTask1, 50);
//	scheduler.arrange_task(myTask2, 150);
	scheduler.register_interest(&myHandler, sync::message::APPLICATION, sync::message::MASK_TYPE);

	scheduler.put_message(sync::create_message(sync::message::SYSTEM, 1));
	scheduler.put_message(sync::create_message(sync::message::SYSTEM, 2));
	scheduler.put_message(sync::create_message(sync::message::SYSTEM, 3));
	sync::sleep(5000);

	return 0;
}
