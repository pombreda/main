#ifndef BASIS_SYS_SYNC_SYNC_FWD_HPP_
#define BASIS_SYS_SYNC_SYNC_FWD_HPP_

namespace sync
{
	namespace message
	{
		using param_type = size_t;

		static const param_type MASK_ALL_TYPES = ~static_cast<param_type>(0xFF);
		static const param_type MASK_TYPE = static_cast<param_type>(0xFF);

		enum Type {
			SYSTEM      = 0x01,
			APPLICATION = 0x02,
			THREAD      = 0x03,
		};

		enum Code {
			SYSTEM_EXIT             = 0x01,
			SYSTEM_SCHEDULER_STOP   = 0x02,
			SYSTEM_SCHEDULER_UPDATE = 0x03,
			SYSTEM_THREAD_STOP      = 0x04,

			APPLICATION_TIMER       = 0x01,
		};

		class SimpleMessage;
	}

	namespace queue
	{
		class Interface;
	}

	class MessageManagerI;
	class Observer;
	class Observable;

	using timer_t = int64_t;
	const timer_t TIMEOUT_INFINITE = INT64_MAX;

	enum class WaitResult_t : size_t {
		SUCCESS   = WAIT_OBJECT_0,
		APC       = WAIT_IO_COMPLETION,
		TIMEOUT   = WAIT_TIMEOUT,
		FAILED    = WAIT_FAILED,
		ABANDONED = WAIT_ABANDONED,
	};
}

#endif
