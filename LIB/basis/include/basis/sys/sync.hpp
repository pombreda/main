#ifndef BASIS_SYS_SYNC_HPP_
#define BASIS_SYS_SYNC_HPP_

#include <basis/configure.hpp>
#include <basis/ext/pattern.hpp>

#include <basis/simstd/memory>

namespace sync
{
	class MessageI;
	class QueueI;
	class MessageManagerI;
	class Observer;
	class Observable;

	using Timeout_t = ssize_t;

	const Timeout_t WAIT_FOREVER = INFINITE;

	enum class WaitResult_t : size_t {
		SUCCESS   = WAIT_OBJECT_0,
		APC       = WAIT_IO_COMPLETION,
		TIMEOUT   = WAIT_TIMEOUT,
		FAILED    = WAIT_FAILED,
		ABANDONED = WAIT_ABANDONED,
	};
}

namespace totext
{
	const wchar_t* c_str(sync::WaitResult_t waitResult);
}

#include <basis/sys/~sync/CriticalSection.hpp>
#include <basis/sys/~sync/Mutex.hpp>
#include <basis/sys/~sync/Semaphore.hpp>
#include <basis/sys/~sync/SRWlock.hpp>
#include <basis/sys/~sync/Message.hpp>
#include <basis/sys/~sync/Queue.hpp>
#include <basis/sys/~sync/Observer.hpp>
#include <basis/sys/~sync/Delivery.hpp>

#endif
