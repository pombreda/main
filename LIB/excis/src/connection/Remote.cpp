#include <excis/connection.hpp>
#include <excis/exception.hpp>
#include <excis/dll.hpp>
#include <basis/os/mpr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/string>

namespace
{
	ustring make_IPC_string(const ustring& host)
	{
		auto prefix = (host[0] != PATH_SEPARATOR_C || host[1] != PATH_SEPARATOR_C) ? NETWORK_PATH_PREFIX : EMPTY_STR;

		wchar_t ipc[MAX_PATH];
		safe_snprintf(ipc, lengthof(ipc), L"%s%s%s", prefix, host.c_str(), L"\\IPC$");

		return ustring(ipc);
	}
}

namespace connection
{
	struct RemoteImpl:
		public RemoteI
	{
		~RemoteImpl();

		RemoteImpl(const ustring& host, const wchar_t* user, const wchar_t* pass);

	private:
		ustring do_get_host() const override;
		void do_connect(const ustring& host, const wchar_t* user, const wchar_t* pass) override;
		void do_disconnect() override;

		ustring m_host;
		bool m_connected;
	};

	RemoteImpl::~RemoteImpl()
	{
		LogTraceObj(L"begin\n");
		do_disconnect();
		LogTraceObj(L"end\n");
	}

	RemoteImpl::RemoteImpl(const ustring& host, const wchar_t* user, const wchar_t* pass) :
		m_connected(false)
	{
		LogTraceObj(L"begin ('%s', %s)\n", host.c_str(), user);
		do_connect(host, user, pass);
		LogTraceObj(L"end\n");
	}

	ustring RemoteImpl::do_get_host() const
	{
		return m_host;
	}

	void RemoteImpl::do_connect(const ustring& host, PCWSTR user, PCWSTR pass)
	{
		do_disconnect();
		if (!host.empty()) {
			NETRESOURCE NetRes;
			memory::zero(NetRes);
			ustring ipc = make_IPC_string(host);
			LogDebug(L"'%s@%s' \n", user, host.c_str());
			NetRes.dwType = RESOURCETYPE_ANY;
			NetRes.lpRemoteName = (PWSTR)ipc.c_str();
			if (cstr::is_empty(user)) {
				user = nullptr;
				pass = nullptr;
			}
			CheckApiError(os::mpr_dll::inst().WNetAddConnection2W(&NetRes, pass, user, 0));
			m_host = host;
			m_connected = true;
		}
		//	CheckApiError(ERROR_BAD_NETPATH);
	}

	void RemoteImpl::do_disconnect()
	{
		if (m_connected) {
			ustring ipc = make_IPC_string(m_host);
			LogDebug(L"'%s'\n", m_host.c_str());
			CheckApiError(os::mpr_dll::inst().WNetCancelConnection2W(ipc.c_str(), 0, FALSE));
			m_connected = false;
		}
		m_host.clear();
	}
}

namespace connection
{
	ustring RemoteI::get_host() const
	{
		return do_get_host();
	}

	void RemoteI::connect()
	{
		do_connect(ustring(), nullptr, nullptr);
	}

	void RemoteI::connect(const ustring& host, const wchar_t* user, const wchar_t* pass)
	{
		do_connect(host, user, pass);
	}

	Remote create()
	{
		LogTraceLn();
		return simstd::make_unique<RemoteImpl>(ustring(), nullptr, nullptr);
	}

	Remote create(const ustring& host, const wchar_t* user, const wchar_t* pass)
	{
		LogTrace(L"('%s', %s)\n", host.c_str(), user);
		return simstd::make_unique<RemoteImpl>(host, user, pass);
	}
}
