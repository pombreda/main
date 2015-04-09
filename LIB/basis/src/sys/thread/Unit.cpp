#include <basis/sys/thread.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/simstd/algorithm>
#include <basis/simstd/string>

namespace thread {

	LogRegister(L"threads");

	Unit::~Unit() noexcept
	{
		LogTraceObjLn();
		if (m_handle) {
			LogTrace(L"[%u] -> exitcode: %Iu\n", m_id, get_exitcode());
			::CloseHandle(m_handle);
		}
	}

	Unit::Unit(Routine* routine, bool suspended, size_t stack_size) :
		m_routine(routine),
		m_handle()
	{
		LogTraceObj(L"(%p, %u, %Iu)\n", routine, suspended, stack_size);
		m_handle = ::CreateThread(nullptr, stack_size, Routine::run_thread, routine, suspended ? CREATE_SUSPENDED : 0, &m_id);
		LogDebugIf(is_valid(), L"[%u]\n", m_id);
		LogFatalIf(!is_valid(), L"can't create thread (%p, %u, %Iu) -> %s\n", routine, suspended, stack_size, totext::api_error().c_str());
	}

	Unit::Unit(Unit&& right) :
		m_routine(nullptr),
		m_handle(nullptr),
		m_id()
	{
		swap(right);
	}

	Unit & Unit::operator =(Unit&& right)
	{
		if (this != &right)
			Unit(simstd::move(right)).swap(*this);
		return *this;
	}

	void Unit::swap(Unit& right) noexcept
	{
		using simstd::swap;
		swap(m_handle, right.m_handle);
		swap(m_routine, right.m_routine);
		swap(m_id, right.m_id);
	}

	bool Unit::is_valid() const
	{
		return m_handle && m_handle != INVALID_HANDLE_VALUE;
	}

	bool Unit::alert()
	{
		bool ret = ::QueueUserAPC(Routine::alert_thread, m_handle, (ULONG_PTR )m_routine);
		LogDebugIf(ret, L"[%u]\n", m_id);
		LogErrorIf(!ret, L"[%u]-> %s\n", m_id, totext::api_error().c_str());
		return ret;
	}

	bool Unit::set_priority(Priority prio)
	{
		bool ret = ::SetThreadPriority(m_handle, (int)prio);
		LogDebugIf(ret, L"[%u] ('%s')\n", m_id, totext::c_str(prio));
		LogErrorIf(!ret, L"[%u] ('%s') -> %s\n", m_id, totext::c_str(prio), totext::api_error().c_str());
		return ret;
	}

	size_t Unit::get_exitcode() const
	{
		DWORD ret;
		WINBOOL good = ::GetExitCodeThread(m_handle, &ret);
		LogDebugIf(good, L"[%u] -> %u\n", m_id, ret);
		LogErrorIf(!good, L"[%u] -> %s\n", m_id, totext::api_error().c_str());
		return ret;
	}

	Id Unit::get_id() const
	{
		return m_id;
	}

	Unit::handle_type Unit::get_handle() const
	{
		return m_handle;
	}

	Routine * Unit::get_routine() const
	{
		return m_routine;
	}

	Priority Unit::get_priority() const
	{
		Priority prio = (Priority)::GetThreadPriority(m_handle);
		LogTrace(L"[%u] -> '%s'\n", m_id, totext::c_str(prio));
		return prio;
	}

	bool Unit::suspend() const
	{
		bool ret = ::SuspendThread(m_handle) != (DWORD)-1;
		LogDebugIf(ret, L"[%u]\n", m_id);
		LogErrorIf(!ret, L"[%u] -> %s\n", m_id, totext::api_error().c_str());
		return ret;
	}

	bool Unit::resume() const
	{
		bool ret = ::ResumeThread(m_handle) != (DWORD)-1;
		LogDebugIf(ret, L"[%u]\n", m_id);
		LogErrorIf(!ret, L"[%u] -> %s\n", m_id, totext::api_error().c_str());
		return ret;
	}

	sync::WaitResult_t Unit::wait(int64_t timeout) const
	{
		LogTrace(L"[%u] (%Id)\n", m_id, timeout);
		return (sync::WaitResult_t)::WaitForSingleObjectEx(m_handle, timeout, true);
	}
}
