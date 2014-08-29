﻿//#include <libbase/bit.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

#include <excis/connection.hpp>
#include <excis/service.hpp>
#include <excis/exception.hpp>

#include <vector>

namespace {
//	const size_t SERVICE_CONFIG_DELAYED_AUTO_START_INFO = 3;
}

namespace Ext {

	Service::Logon_t::Logon_t():
		serviceStartName(nullptr),
		password(nullptr)
	{
	}

	Service::Logon_t::Logon_t(PCWSTR user, PCWSTR pass):
		serviceStartName(user),
		password(pass)
	{
	}

	///===================================================================================== Service
	Service::~Service() {
//		LogDebug(L"handle: %p\n", m_hndl);
		if (m_hndl)
			::CloseServiceHandle(m_hndl);
		m_hndl = nullptr;
	}

	Service::Service(SC_HANDLE scm, PCWSTR name, ACCESS_MASK access):
		m_hndl(CheckHandleErr(::OpenServiceW(scm, name, access)))
	{
//		LogDebug(L"name: %s handle: %p\n", name, m_hndl);
	}

	Service::Service(Service && right):
		m_hndl(right.m_hndl)
	{
//		LogTrace();
		right.m_hndl = nullptr;
	}

	Service & Service::operator = (Service && right) {
//		LogTrace();
		m_hndl = right.m_hndl;
		right.m_hndl = nullptr;
		return *this;
	}

	memory::auto_buf<LPQUERY_SERVICE_CONFIGW> Service::QueryConfig() const {
//		LogDebug(L"handle: %p\n", m_hndl);
		DWORD bytesNeeded = 0;
		CheckApi(!::QueryServiceConfigW(m_hndl, nullptr, 0, &bytesNeeded) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER);
		memory::auto_buf<LPQUERY_SERVICE_CONFIGW> buf(bytesNeeded);
		CheckApi(::QueryServiceConfigW(m_hndl, buf, buf.size(), &bytesNeeded));
		return buf;
	}

	memory::auto_buf<PBYTE> Service::QueryConfig2(DWORD level) const {
		DWORD bytesNeeded = 0;
		CheckApi(!::QueryServiceConfig2W(m_hndl, level, nullptr, 0, &bytesNeeded) && ::GetLastError() == ERROR_INSUFFICIENT_BUFFER);
		memory::auto_buf<PBYTE> buf(bytesNeeded);
		CheckApi(::QueryServiceConfig2W(m_hndl, level, buf, buf.size(), &bytesNeeded));
		return buf;
	}

	void Service::del() {
		CheckApi(::DeleteService(m_hndl));
	}

	//	template<typename Functor>
	//	void WaitForState(DWORD state, DWORD dwTimeout, Functor &func, PVOID param = nullptr) const {
	//		DWORD	dwStartTime = ::GetTickCount();
	//		DWORD	dwBytesNeeded;
	//		SERVICE_STATUS_PROCESS ssp = {0};
	//		while (true) {
	//			CheckApi(::QueryServiceStatusEx(m_hndl, SC_STATUS_PROCESS_INFO, (PBYTE)&ssp, sizeof(ssp), &dwBytesNeeded));
	//			if (ssp.dwCurrentState == state)
	//				break;
	//			if (::GetTickCount() - dwStartTime > dwTimeout)
	//				throw	ApiError(WAIT_TIMEOUT);
	//			func(state, ::GetTickCount() - dwStartTime, param);
	//		}
	//	}

	Service & Service::start() {
		CheckApi(::StartServiceW(m_hndl, 0, nullptr) || ::GetLastError() == ERROR_SERVICE_ALREADY_RUNNING);
		return *this;
	}

	Service & Service::stop() {
		SERVICE_STATUS ss;
		CheckApi(::ControlService(m_hndl, SERVICE_CONTROL_STOP, &ss) || ::GetLastError() == ERROR_SERVICE_NOT_ACTIVE);
		return *this;
	}

	Service & Service::contin() {
		SERVICE_STATUS ss;
		CheckApi(::ControlService(m_hndl, SERVICE_CONTROL_CONTINUE, &ss));
		return *this;
	}

	Service & Service::pause() {
		SERVICE_STATUS ss;
		CheckApi(::ControlService(m_hndl, SERVICE_CONTROL_PAUSE, &ss));
		return *this;
	}

	Service & Service::set_config(const Service::Config_t & info) {
		CheckApi(::ChangeServiceConfigW(
			m_hndl,
			info.serviceType,
			info.startType,
			info.errorControl,
			info.binaryPathName,
			info.loadOrderGroup,
			info.tagId,
			info.dependencies,
			nullptr,
			nullptr,
			info.displayName
		));
		if (info.delayedStart != SERVICE_NO_CHANGE)
			set_delayed(info.delayedStart);
		return *this;
	}

	Service & Service::set_logon(const Service::Logon_t & info) {
		CheckApi(::ChangeServiceConfigW(
			m_hndl,
			info.serviceType,
			info.startType,
			info.errorControl,
			info.binaryPathName,
			info.loadOrderGroup,
			info.tagId,
			info.dependencies,
			info.serviceStartName,
			info.password,
			info.displayName
		));
		return *this;
	}

	Service & Service::set_description(PCWSTR info) {
		SERVICE_DESCRIPTIONW descr = {(PWSTR)info};
		CheckApi(::ChangeServiceConfig2W(m_hndl, SERVICE_CONFIG_DESCRIPTION, &descr));
		return *this;
	}

	Service & Service::set_delayed(bool state) {
		SERVICE_DELAYED_AUTO_START_INFO info = {state};
		CheckApi(::ChangeServiceConfig2W(m_hndl, SERVICE_CONFIG_DELAYED_AUTO_START_INFO, &info));
		return *this;
	}

	Service & Service::wait_state(State_t state, DWORD dwTimeout) {
		DWORD dwStartTime = ::GetTickCount();
		while (true) {
			Service::Status_t ssp(get_status());
			if (ssp.dwCurrentState == (DWORD)state)
				break;
			if (::GetTickCount() - dwStartTime > dwTimeout)
				CheckApiError(WAIT_TIMEOUT);
			::Sleep(500);
		};
		return *this;
	}

	ustring Service::get_description() const {
		memory::auto_buf<PBYTE> conf(QueryConfig2(SERVICE_CONFIG_DESCRIPTION));
		LPSERVICE_DESCRIPTIONW lpsd = (LPSERVICE_DESCRIPTIONW)conf.data();
		return ustring((lpsd->lpDescription) ? lpsd->lpDescription : EMPTY_STR);
	}

	bool Service::get_delayed() const {
		bool ret = false;
		try {
			memory::auto_buf<PBYTE> conf(QueryConfig2(SERVICE_CONFIG_DELAYED_AUTO_START_INFO));
			LPSERVICE_DELAYED_AUTO_START_INFO lpsd = (LPSERVICE_DELAYED_AUTO_START_INFO)conf.data();
			ret = lpsd->fDelayedAutostart;
		} catch (exception::AbstractError & e) {
		}
		return ret;
	}

	Service::Status_t Service::get_status() const {
		Service::Status_t ret;
		DWORD bytesNeeded;
		CheckApi(::QueryServiceStatusEx(m_hndl, SC_STATUS_PROCESS_INFO, (PBYTE)&ret, sizeof(ret), &bytesNeeded));
		return ret;
	}

	Service::State_t Service::get_state() const {
		Service::Status_t tmp(get_status());
		return (Service::State_t)tmp.dwCurrentState;
	}

	Service::Start_t Service::get_start_type() const {
		return (Service::Start_t)QueryConfig()->dwStartType;
	}

	DWORD Service::get_type() const {
		return QueryConfig()->dwServiceType;
	}

	ustring Service::get_user() const {
#ifdef _MSC_VER
		PCWSTR ret = QueryConfig()->lpServiceStartName ? QueryConfig()->lpServiceStartName : EMPTY_STR;
		return ustring(ret);
#else
		return ustring((QueryConfig()->lpServiceStartName) ?: EMPTY_STR);
#endif
	}

	void Service::del(SC_HANDLE scm, PCWSTR name) {
		Service(scm, name, SERVICE_STOP | DELETE).stop().del();
	}

	Service Service::start(SC_HANDLE scm, PCWSTR name) {
		return simstd::move(Service(scm, name, SERVICE_START | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).start());
	}

	Service Service::stop(SC_HANDLE scm, PCWSTR name) {
		return simstd::move(Service(scm, name, SERVICE_STOP | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).stop());
	}

	Service Service::restart(SC_HANDLE scm, PCWSTR name) {
		return simstd::move(Service(scm, name, SERVICE_STOP | SERVICE_START | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).stop().wait_state(State_t::STOPPED, 30000).start());
	}

	Service Service::contin(SC_HANDLE scm, PCWSTR name) {
		return simstd::move(Service(scm, name, SERVICE_START | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).contin());
	}

	Service Service::pause(SC_HANDLE scm, PCWSTR name) {
		return simstd::move(Service(scm, name, SERVICE_STOP | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).pause());
	}

	Service Service::set_config(SC_HANDLE scm, PCWSTR name, const Service::Config_t & info) {
		return simstd::move(Service(scm, name, SERVICE_CHANGE_CONFIG | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).set_config(info));
	}

	Service Service::set_logon(SC_HANDLE scm, PCWSTR name, const Service::Logon_t & info) {
		return simstd::move(Service(scm, name, SERVICE_CHANGE_CONFIG | SERVICE_QUERY_CONFIG | SERVICE_QUERY_STATUS).set_logon(info));
	}

	Service::Status_t Service::get_status(SC_HANDLE scm, PCWSTR name) {
		return Service(scm, name, SERVICE_QUERY_STATUS).get_status();
	}

	Service::State_t Service::get_state(SC_HANDLE scm, PCWSTR name) {
		return (Service::State_t)get_status(scm, name).dwCurrentState;
	}

	Service::Start_t Service::get_start_type(SC_HANDLE scm, PCWSTR name) {
		return Service(scm, name, SERVICE_QUERY_CONFIG).get_start_type();
	}

	ustring Service::get_description(SC_HANDLE scm, PCWSTR name) {
		return Service(scm, name, SERVICE_QUERY_CONFIG).get_description();
	}

}