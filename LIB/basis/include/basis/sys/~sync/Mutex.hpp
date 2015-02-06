#ifndef BASIS_SYS_SYNC_MUTEX_HPP_
#define BASIS_SYS_SYNC_MUTEX_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	class Mutex: private pattern::Uncopyable
	{
		struct native_impl_type;

	public:
		using native_handle_type = native_impl_type*;

		~Mutex();

		Mutex(const wchar_t* name = EMPTY_STR);

		void lock();

		bool try_lock(size_t timeout_millisec = 0);

		void unlock();

		native_handle_type native_handle();

	private:
		native_handle_type m_handle;
	};

}

#endif
