#include <tests.hpp>

#include <basis/configure.hpp>
#include <basis/sys/logger.hpp>

//class WriterRoutine: public thread::Routine
//{
//public:
//	~WriterRoutine()
//	{
//		LogTraceObjLn();
//	}
//
//	WriterRoutine(sync::Queue* queue, ssize_t num):
//		m_queue(queue),
//		m_num(num)
//	{
//		LogTraceObjLn();
//	}
//
//	size_t run(void*) override
//	{
//		LogDebug(L"Start routine\n");
//		::Sleep(3000);
//		sync::MessageI message;
//		m_queue->get_message(message, 5000);
//		LogDebug(L"Exit routine\n");
//		return m_num;
//	}
//
//private:
//	sync::Queue* m_queue;
//	ssize_t m_num;
//};

void test_lock_rw()
{


}
