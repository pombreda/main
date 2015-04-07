#ifndef BASIS_SYS_SYNC_SYNC_FWD_HPP_
#define BASIS_SYS_SYNC_SYNC_FWD_HPP_

namespace sync
{
	class MessageI;
	class QueueI;
	class MessageManagerI;
	class Observer;
	class Observable;

	const unsigned WAIT_FOREVER = INFINITE;

	enum class WaitResult_t : size_t {
		SUCCESS   = WAIT_OBJECT_0,
		APC       = WAIT_IO_COMPLETION,
		TIMEOUT   = WAIT_TIMEOUT,
		FAILED    = WAIT_FAILED,
		ABANDONED = WAIT_ABANDONED,
	};

	inline void sleep(size_t timeout_ms) noexcept {::Sleep(timeout_ms);}

	inline int64_t now() noexcept {return ::GetTickCount64();}
}

namespace totext
{
	const wchar_t* c_str(sync::WaitResult_t waitResult);
}

#endif
