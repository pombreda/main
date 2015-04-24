#include <basis/sys/thread.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/simstd/algorithm>
#include "UnitImpl.hpp"

namespace thread
{
	LogRegister(L"threads");

	UnitImpl::~UnitImpl() noexcept
	{
		LogTraceObjLn();
		if (m_handle) {
			LogTrace(L"[%u] -> exitcode: %Iu\n", m_id, get_exitcode());
			::CloseHandle(m_handle);
		}
	}

	UnitImpl::UnitImpl(const wchar_t* name, Routine* routine, bool suspended, size_t stack_size) noexcept :
		m_routine(routine),
		m_handle(),
		name(name)
	{
		LogTraceObj(L"('%s', %p, %u, %Iu)\n", name, routine, suspended, stack_size);
		m_handle = static_cast<handle_type>(::CreateThread(nullptr, stack_size, Routine::run_thread, routine, suspended ? CREATE_SUSPENDED : 0, &m_id));
		LogDebugIf(is_valid(), L"[%u]\n", m_id);
		LogFatalIf(!is_valid(), L"can't create thread (%p, %u, %Iu) -> %s\n", routine, suspended, stack_size, totext::api_error().c_str());
	}

	bool UnitImpl::is_valid() const noexcept
	{
		return m_handle && static_cast<HANDLE>(m_handle) != INVALID_HANDLE_VALUE;
	}

	bool UnitImpl::alert() noexcept
	{
		bool ret = ::QueueUserAPC(Routine::alert_thread, m_handle, (ULONG_PTR )m_routine);
		LogDebugIf(ret, L"[%u]\n", m_id);
		LogErrorIf(!ret, L"['%s', %u]-> %s\n", get_name(), m_id, totext::api_error().c_str());
		return ret;
	}

	bool UnitImpl::set_priority(Priority prio) noexcept
	{
		bool ret = ::SetThreadPriority(m_handle, (int)prio);
		LogDebugIf(ret, L"[%u] (%Id)\n", m_id, ssize_t(prio));
		LogErrorIf(!ret, L"['%s', %u] ('%s') -> %s\n", get_name(), m_id, totext::c_str(prio), totext::api_error().c_str());
		return ret;
	}

	size_t UnitImpl::get_exitcode() const noexcept
	{
		DWORD ret;
		WINBOOL good = ::GetExitCodeThread(m_handle, &ret);
		LogDebugIf(good, L"[%u] -> %u\n", m_id, ret);
		LogErrorIf(!good, L"['%s', %u] -> %s\n", get_name(), m_id, totext::api_error().c_str());
		return ret;
	}

	Id UnitImpl::get_id() const noexcept
	{
		return m_id;
	}

	UnitImpl::handle_type UnitImpl::get_handle() const noexcept
	{
		return m_handle;
	}

	Routine* UnitImpl::get_routine() const noexcept
	{
		return m_routine;
	}

	const wchar_t* UnitImpl::get_name() const noexcept
	{
		return name.c_str();
	}

	Priority UnitImpl::get_priority() const noexcept
	{
		Priority prio = (Priority)::GetThreadPriority(m_handle);
		LogTrace(L"[%u] -> %Id\n", m_id, ssize_t(prio));
		return prio;
	}

	bool UnitImpl::suspend() const noexcept
	{
		bool ret = ::SuspendThread(m_handle) != static_cast<DWORD>(-1);
		LogDebugIf(ret, L"[%u]\n", m_id);
		LogErrorIf(!ret, L"['%s', %u] -> %s\n", get_name(), m_id, totext::api_error().c_str());
		return ret;
	}

	bool UnitImpl::resume() const noexcept
	{
		bool ret = ::ResumeThread(m_handle) != static_cast<DWORD>(-1);
		LogDebugIf(ret, L"[%u]\n", m_id);
		LogErrorIf(!ret, L"['%s', %u] -> %s\n", get_name(), m_id, totext::api_error().c_str());
		return ret;
	}

	sync::WaitResult_t UnitImpl::wait(int64_t timeout) const noexcept
	{
		LogTrace(L"[%u] (%I64d)\n", m_id, timeout);
		return (sync::WaitResult_t)::WaitForSingleObjectEx(m_handle, timeout, true);
	}

	Unit create(const wchar_t* name, Routine* routine, bool suspended, size_t stack_size)
	{
		return simstd::make_unique<UnitImpl>(name, routine, suspended, stack_size);
	}
}
