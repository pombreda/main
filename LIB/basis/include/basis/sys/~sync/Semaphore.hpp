#ifndef BASIS_SYS_SYNC_SEMAPHORE_HPP_
#define BASIS_SYS_SYNC_SEMAPHORE_HPP_

namespace sync
{
	class Semaphore: private pattern::Uncopyable
	{
		struct native_imp_type;

	public:
		using native_handle_type = native_imp_type*;

		~Semaphore() noexcept;

		Semaphore(const wchar_t* name = nullptr) noexcept;

		void lock() noexcept;

		bool try_lock(size_t timeout_millisec = 0) noexcept;

		WaitResult_t try_lock_ex(size_t timeout_millisec = 0) noexcept;

		void unlock(size_t cnt = 1) noexcept;

		native_handle_type native_handle() noexcept;

	private:
		native_handle_type handle;
	};
}

#endif
