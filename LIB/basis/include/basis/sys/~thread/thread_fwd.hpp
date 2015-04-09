#ifndef BASIS_SYS_THREAD_THREAD_FWD_HPP_
#define BASIS_SYS_THREAD_THREAD_FWD_HPP_

//namespace thread {
//	LogDeclare();
//}

namespace thread
{
	enum class Priority: ssize_t {
		IDLE          = THREAD_PRIORITY_IDLE,
		LOWEST        = THREAD_PRIORITY_LOWEST,
		BELOW_NORMAL  = THREAD_PRIORITY_BELOW_NORMAL,
		NORMAL        = THREAD_PRIORITY_NORMAL,
		ABOVE_NORMAL  = THREAD_PRIORITY_ABOVE_NORMAL,
		HIGHEST       = THREAD_PRIORITY_HIGHEST,
		TIME_CRITICAL = THREAD_PRIORITY_TIME_CRITICAL,
	};

	enum class IoPriority: ssize_t {
		VERY_LOW,
		LOW,
		NORMAL,
		HIGH,
		CRITICAL,
	};

	using Id = DWORD;

	Id get_id();
}

namespace totext
{
	const wchar_t* c_str(thread::Priority prio);

	const wchar_t* c_str(thread::IoPriority prio);
}

#endif
