#include <basis/tst.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

class MyTask: public sync::Task
{
	const wchar_t* get_name() const override
	{
		return L"MyTask";
	}

	bool do_execute(bool exiting) override
	{
		LogTraceObj(L"(%d)\n", exiting);
		return true;
	}
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

	MyTask myTask;
	MyEventHandler myHandler;

	sync::Scheduler scheduler(L"tst::_Scheduler");
	sync::sleep(1000);
	scheduler.arrange_task(&myTask, 50);
	scheduler.register_interest(&myHandler, sync::MessageI::SYSTEM, sync::MessageI::MASK_TYPE);

	scheduler.put_message(sync::create_message(sync::MessageI::SYSTEM, 1));
	scheduler.put_message(sync::create_message(sync::MessageI::SYSTEM, 2));
	scheduler.put_message(sync::create_message(sync::MessageI::SYSTEM, 3));
	sync::sleep(5000);

	return 0;
}
