#include <basis/tst.hpp>
#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

ssize_t tst::_sync()
{
	auto queue1 = sync::create_queue(L"QueueTest1");
	LogTrace();
	LogTrace1(L"");

	queue1->empty();

	return 0;
}
