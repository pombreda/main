#include <basis/sys/thread.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/simstd/string>

namespace thread {

	Pool::Pool()
	{
		LogTraceLn();
	}

	Pool::~Pool()
	{
		LogTraceLn();
	}

	sync::WaitResult_t Pool::wait_all(size_t timeout) const
	{
		LogTrace(L"timeout: %Id\n", timeout);
		sync::WaitResult_t ret = (sync::WaitResult_t)::WaitForMultipleObjectsEx(m_handles.size(), &m_handles[0], TRUE, timeout, TRUE);
		LogErrorIf(ret == sync::WaitResult_t::FAILED, L"-> %s\n", totext::api_error().c_str());
		LogTrace(L"ret: '%s'\n", totext::c_str(ret));
		return ret;
	}

	sync::WaitResult_t Pool::wait_any(size_t & index, size_t timeout) const
	{
		LogTrace(L"index: %Iu, timeout: %Id\n", index, timeout);
		DWORD result = ::WaitForMultipleObjectsEx(m_handles.size(), &m_handles[0], FALSE, timeout, TRUE);
		sync::WaitResult_t ret = sync::WaitResult_t::FAILED;
		if (result == WAIT_IO_COMPLETION || result == WAIT_TIMEOUT || result == WAIT_FAILED) {
			ret = (sync::WaitResult_t)result;
		} else if (result >= WAIT_ABANDONED_0) {
			index = result - WAIT_ABANDONED_0;
			ret = sync::WaitResult_t::ABANDONED;
		} else {
			index = result - WAIT_OBJECT_0;
			ret = sync::WaitResult_t::SUCCESS;
		}
		LogErrorIf(ret == sync::WaitResult_t::FAILED, L"%s\n", totext::api_error().c_str());
		LogTrace(L"wait: %Id, ret: '%s', index: %Iu\n", timeout, totext::c_str(ret), index);
		return ret;
	}

}
