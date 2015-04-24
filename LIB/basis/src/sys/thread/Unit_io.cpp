#include <basis/sys/thread.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/os/ntdll.hpp>
#include "UnitImpl.hpp"

namespace thread
{
	bool UnitImpl::set_io_priority(IoPriority prio)
	{
		ULONG p = (ULONG)prio;
		NTSTATUS ret = os::ntdll_dll::inst().NtSetInformationThread(m_handle, os::ThreadIoPriority, &p, sizeof(p));
		LogDebugIf(!ret, L"id: %u, prio: '%s'\n", m_id, totext::c_str(prio));
		LogErrorIf( ret, L"id: %u, prio: '%s' -> %s\n", m_id, totext::c_str(prio), totext::nt_status(ret).c_str());
		return ret;
	}

	IoPriority UnitImpl::get_io_priority() const
	{
		ULONG prio = 0;
		NTSTATUS ret = os::ntdll_dll::inst().NtQueryInformationThread(m_handle, os::ThreadIoPriority, &prio, sizeof(prio), nullptr);
		LogDebugIf(!ret, L"id: %u -> '%s'\n", m_id, totext::c_str((IoPriority)prio));
		LogErrorIf( ret, L"id: %u -> '%s'\n", m_id, totext::nt_status(ret).c_str());
		return (IoPriority)prio;
	}
}
