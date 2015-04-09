#include <basis/sys/sync.hpp>

namespace sync
{
	void sleep(int64_t timeout_ms) noexcept
	{
		::Sleep(timeout_ms);
	}

	int64_t now() noexcept
	{
		return ::GetTickCount64();
	}
}
