#include <basis/configure.hpp>
#include <basis/sys/~sync/Semaphore.hpp>

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

	void Semaphore::lock() noexcept
	{
		::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(handle), INFINITE, true);
	}

	bool Semaphore::try_lock(size_t timeout_millisec) noexcept
	{
		return WAIT_OBJECT_0 == ::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(handle), timeout_millisec, true);
	}

	WaitResult_t Semaphore::try_lock_ex(size_t timeout_millisec) noexcept
	{
		return static_cast<WaitResult_t>(::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(handle), timeout_millisec, true));
	}

	void Semaphore::unlock(size_t cnt) noexcept
	{
		::ReleaseSemaphore(reinterpret_cast<HANDLE>(handle), cnt, nullptr);
	}

	Semaphore::native_handle_type Semaphore::native_handle() noexcept
	{
		return handle;
	}
}
