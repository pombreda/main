#include <excis/service.hpp>
#include <excis/exception.hpp>
#include <basis/sys/logger.hpp>

namespace service
{
	namespace
	{
		SC_HANDLE service_manager_open(const connection::Remote& conn, ACCESS_MASK acc)
		{
			return CheckHandleErr(::OpenSCManagerW(conn->get_host().c_str(), nullptr, acc));
		}

		void service_manager_close(SC_HANDLE scm)
		{
			if (scm)
				::CloseServiceHandle(scm);
		}
	}

	Manager::~Manager()
	{
		LogTraceObjLn();
		service_manager_close(m_hndl);
	}

	Manager::Manager(const connection::Remote& conn, ACCESS_MASK acc) :
		m_hndl()
	{
		LogTraceObj(L"begin ('%s', 0x%X)\n", conn->get_host().c_str(), acc);
		m_hndl = service_manager_open(conn, acc);
		LogTraceObj(L"end\n");
	}

	Manager::Manager(Manager&& other):
		m_hndl(other.m_hndl)
	{
		LogTraceObjLn();
		other.m_hndl = nullptr;
	}

	Manager& Manager::operator =(Manager&& other)
	{
		LogTraceObjLn();
		swap(other);
		return *this;
	}

	void Manager::swap(Manager& other)
	{
		using simstd::swap;
		swap(m_hndl, other.m_hndl);
	}

	Manager::operator SC_HANDLE() const
	{
		return m_hndl;
	}

	SC_HANDLE Manager::get_handle() const
	{
		return m_hndl;
	}

	void Manager::reconnect(const connection::Remote& conn, ACCESS_MASK acc)
	{
		LogTraceObj(L"('%s', 0x%X)\n", conn->get_host().c_str(), acc);
		SC_HANDLE l_hndl = service_manager_open(conn, acc);

		using simstd::swap;
		swap(m_hndl, l_hndl);

		service_manager_close(l_hndl);
	}

	bool Manager::is_exist(const wchar_t* name) const
	{
		LogTrace(L"(%s)\n", name);
		SC_HANDLE hndl = ::OpenServiceW(m_hndl, name, SERVICE_QUERY_STATUS);
		if (hndl)
			::CloseServiceHandle(hndl);
		return hndl;
	}
}
