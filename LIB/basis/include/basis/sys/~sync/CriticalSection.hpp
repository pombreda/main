#ifndef BASIS_SYS_SYNC_CRITICALSECTION_HPP_
#define BASIS_SYS_SYNC_CRITICALSECTION_HPP_

namespace sync
{
	class CriticalSection: private pattern::Uncopyable
	{
		using native_impl_type = CRITICAL_SECTION;

	public:
		using native_handle_type = native_impl_type*;

		~CriticalSection()
		{
			::DeleteCriticalSection(&impl);
		}

		CriticalSection()
		{
			::InitializeCriticalSection(&impl);
		}

		void lock() const noexcept
		{
			::EnterCriticalSection(&impl);
		}

		bool try_lock() const noexcept
		{
			return ::TryEnterCriticalSection(&impl);
		}

		void unlock() const noexcept
		{
			::LeaveCriticalSection(&impl);
		}

		native_handle_type native_handle()
		{
			return &impl;
		}

	private:
		mutable native_impl_type impl;
	};
}

#endif
