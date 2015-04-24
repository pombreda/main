#include <tests.hpp>

#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/thread.hpp>

struct Routine: public thread::Routine
{
	Routine(const sync::Queue& queue, ssize_t num):
		m_queue(queue),
		m_num(num)
	{
		LogTraceLn();
	}

	ssize_t run(void *) override
	{
		LogDebug(L"Start routine\n");
		::Sleep(3000);

		sync::Message message;
		m_queue->get_message(message, 5000);

		LogDebug(L"Exit routine\n");
		return m_num;
	}

private:
	const sync::Queue& m_queue;
	ssize_t m_num;
};

void test_threads()
{
	LogTraceLn();
	auto queue = sync::create_queue(L"QueueTestThreads");
	Routine routine1(queue, 100);
	Routine routine2(queue, 200);
	thread::Pool threads;
	threads.create_thread(L"Thread1", &routine1, true);
	threads.create_thread(L"Thread2", &routine2, true);

//	Sleep(5000);
	threads[0]->set_io_priority(thread::IoPriority::VERY_LOW);
	threads[0]->set_io_priority(thread::IoPriority::NORMAL);
	threads[1]->set_io_priority(thread::IoPriority::LOW);
	threads[1]->set_io_priority(thread::IoPriority::HIGH);
	threads[1]->set_io_priority(thread::IoPriority::CRITICAL);

//	Sleep(5000);
	threads[0]->set_priority(thread::Priority::TIME_CRITICAL);
	threads[1]->set_priority(thread::Priority::ABOVE_NORMAL);

	{
		auto message = sync::message::create(1, 2, 3);
		TraceFuncLn();
		TraceFuncLn();
		TraceFuncLn();
		TraceFuncLn();
		queue->put_message(message);
//		queue->put_message(message);
		TraceFuncLn();
		TraceFuncLn();
		TraceFuncLn();
		TraceFuncLn();
	}

	threads[0]->resume();
	threads[1]->resume();

	sync::WaitResult_t ret = sync::WaitResult_t::FAILED;
	do {
		ret = threads.wait_all(1000);
	} while (ret != sync::WaitResult_t::FAILED && ret != sync::WaitResult_t::SUCCESS);

	if (ret == sync::WaitResult_t::SUCCESS) {
		LogInfo(L"threads[0] exited: %d\n", threads[0]->get_exitcode());
		LogInfo(L"threads[1] exited: %d\n", threads[1]->get_exitcode());
	}
}
