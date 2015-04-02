#ifndef BASIS_SYS_SYNC_MUTEX_HPP_
#define BASIS_SYS_SYNC_MUTEX_HPP_

namespace sync
{
	class Mutex: private pattern::Uncopyable
	{
		struct native_impl_type;

	public:
		using native_handle_type = native_impl_type*;

		~Mutex() noexcept;

		Mutex(const wchar_t* name = EMPTY_STR) noexcept;

		void lock() const noexcept;

		bool try_lock(size_t timeout_ms = 0) const noexcept;

		void unlock() const noexcept;

		native_handle_type native_handle() noexcept;

	private:
		native_handle_type handle;
	};
}

#endif
