#include <basis/configure.hpp>
#include <basis/sys/sync.hpp>

namespace sync
{
	Semaphore::~Semaphore() noexcept
	{
		::CloseHandle(reinterpret_cast<HANDLE>(handle));
	}

	Semaphore::Semaphore(const wchar_t* name) noexcept :
		handle(nullptr)
	{
		handle = reinterpret_cast<native_handle_type>(::CreateSemaphoreW(nullptr, 0, LONG_MAX, name));
	}

	void Semaphore::lock() const noexcept
	{
		::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(const_cast<native_handle_type>(handle)), INFINITE, true);
	}

	bool Semaphore::try_lock(size_t timeout_ms) const noexcept
	{
		return WAIT_OBJECT_0 == ::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(const_cast<native_handle_type>(handle)), timeout_ms, true);
	}

	WaitResult_t Semaphore::try_lock_ex(size_t timeout_ms) const noexcept
	{
		return static_cast<WaitResult_t>(::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(const_cast<native_handle_type>(handle)), timeout_ms, true));
	}

	void Semaphore::unlock(size_t cnt) const noexcept
	{
		::ReleaseSemaphore(reinterpret_cast<HANDLE>(const_cast<native_handle_type>(handle)), cnt, nullptr);
	}

	Semaphore::native_handle_type Semaphore::native_handle() noexcept
	{
		return handle;
	}
}
