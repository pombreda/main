#include <basis/sys/thread.hpp>

namespace thread
{
	Id get_id()
	{
		return ::GetCurrentThreadId();
	}
}
