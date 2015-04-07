#include <tests.hpp>

#include <basis/sys/sync.hpp>
//#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/thread.hpp>
#include <basis/simstd/mutex>

sync::CriticalSection* m1;
sync::CriticalSection* m2;

struct LockMutexThead1: public thread::Routine {
	ssize_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			LogTraceLn();
//			simstd::lock(*m2, *m1);
			m2->lock();
			Sleep(10);
//			LogTraceLn();
			m1->lock();
			Sleep(33);
			m1->unlock();
			m2->unlock();
		}

		return 0;
	}
};

struct LockMutexThead2: public thread::Routine {
	ssize_t run(void * data) override
	{
		UNUSED(data);

		while (true) {
			LogTraceLn();
//			simstd::lock(*m1, *m2);
			m1->lock();
			Sleep(10);
//			LogTraceLn();
			m2->lock();
			Sleep(33);
			m1->unlock();
			m2->unlock();
		}

		return 0;
	}
};

void test_lock()
{
	m1 = new sync::CriticalSection;
	m2 = new sync::CriticalSection;

	LockMutexThead1 routine1;
	LockMutexThead2 routine2;

	thread::Pool threads;
	threads.create_thread(&routine1);
	threads.create_thread(&routine1);
	threads.create_thread(&routine1);
	threads.create_thread(&routine1);
	threads.create_thread(&routine2);
	threads.create_thread(&routine2);
	threads.create_thread(&routine2);
	threads.create_thread(&routine2);

	threads.wait_all();

	delete m2;
	delete m1;
}
