#include <basis/sys/sync.hpp>

namespace sync
{
	Mutex::~Mutex() noexcept
	{
		::CloseHandle(reinterpret_cast<HANDLE>(handle));
	}

	Mutex::Mutex(const wchar_t* name) noexcept :
		handle(nullptr)
	{
		handle = reinterpret_cast<native_handle_type>(::CreateMutexW(nullptr, 0, name));
	}

	void Mutex::lock() const noexcept
	{
		::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(const_cast<native_handle_type>(handle)), INFINITE, true);
	}

	bool Mutex::try_lock(size_t timeout_ms) const noexcept
	{
		return WAIT_OBJECT_0 == ::WaitForSingleObjectEx(reinterpret_cast<HANDLE>(const_cast<native_handle_type>(handle)), timeout_ms, true);
	}

	void Mutex::unlock() const noexcept
	{
		::ReleaseMutex(reinterpret_cast<HANDLE>(const_cast<native_handle_type>(handle)));
	}

	Mutex::native_handle_type Mutex::native_handle() noexcept
	{
		return handle;
	}
}
