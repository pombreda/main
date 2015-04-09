#ifndef BASIS_SYS_SYNC_SYNC_FUNCTIONS_HPP_
#define BASIS_SYS_SYNC_SYNC_FUNCTIONS_HPP_

namespace sync
{
	void sleep(int64_t timeout_ms) noexcept;

	int64_t now() noexcept;
}

namespace totext
{
	const wchar_t* c_str(sync::WaitResult_t waitResult);
}

#endif
