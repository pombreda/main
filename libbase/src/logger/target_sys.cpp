﻿#include <libbase/logger.hpp>

#include <libbase/lock.hpp>
#include <libbase/memory.hpp>
#include <libbase/pcstr.hpp>

#include "event_msg.h"

namespace Logger {

	WORD const LogLevelTypes[(int)Level::Logger + 1] = {
		EVENTLOG_SUCCESS,
		EVENTLOG_SUCCESS,
		EVENTLOG_INFORMATION_TYPE,
		EVENTLOG_INFORMATION_TYPE,
		EVENTLOG_WARNING_TYPE,
		EVENTLOG_WARNING_TYPE,
		EVENTLOG_ERROR_TYPE,
		EVENTLOG_ERROR_TYPE,
		EVENTLOG_INFORMATION_TYPE,
	};

	struct LogToSys: public Target_i {
		~LogToSys();

		LogToSys(PCWSTR name, PCWSTR path);

		void out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const override;

		void out(PCWSTR str, size_t size) const override;

		void lock() const override;

		void unlock() const override;

	private:
		static void app_register(PCWSTR name, PCWSTR path);

		Base::auto_destroy<Base::Lock::SyncUnit_i*> m_sync;
		HANDLE m_hndl;
	};

	LogToSys::~LogToSys()
	{
		::DeregisterEventSource(m_hndl);
	}

	LogToSys::LogToSys(PCWSTR name, PCWSTR path) :
		m_sync(Base::Lock::get_CritSection())
	{
		app_register(name, path);
		m_hndl = ::RegisterEventSourceW(nullptr, name);
	}

	void LogToSys::out(const Module_i * /*lgr*/, Level lvl, PCWSTR str, size_t /*size*/) const
	{
		//			PSID user = nullptr;
		//			HANDLE token;
		//			PTOKEN_USER token_user = nullptr;
		//			if (OpenThreadToken(GetCurrentThread(), TOKEN_QUERY, FALSE, &token)) {
		//				token_user = (PTOKEN_USER)DefaultTokenInformation(token, TokenUser);
		//				if (token_user)
		//					user = token_user->User.Sid;
		//				CloseHandle(token);
		//			}
		::ReportEventW(m_hndl, LogLevelTypes[(int)lvl], 0, EV_MSG_STRING, nullptr, 1, 0, &str, nullptr);
		//			free(token_user);
	}

	void LogToSys::out(PCWSTR str, size_t /*size*/) const
	{
		::ReportEventW(m_hndl, LogLevelTypes[(int)get_default_level()], 0, EV_MSG_STRING, nullptr, 1, 0, &str, nullptr);
	}

	void LogToSys::lock() const
	{
		m_sync->lock();
	}

	void LogToSys::unlock() const
	{
		m_sync->release();
	}

	void LogToSys::app_register(PCWSTR name, PCWSTR path)
	{
		wchar_t path_buf[Base::MAX_PATH_LEN], *fullpath = path_buf;
		if (Base::Str::is_empty(path)) {
			::GetModuleFileNameW(0, path_buf, Base::lengthof(path_buf));
		} else {
			fullpath = (PWSTR)path;
		}

		auto key1 = L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\";
		auto key_length = Base::Str::length(key1);
		wchar_t value[Base::MAX_PATH_LEN];
		Base::Str::copy(value, key1, Base::lengthof(value));
		Base::Str::cat(value, name, Base::lengthof(value) - key_length);
		HKEY hKey = nullptr;
		::RegCreateKeyW(HKEY_LOCAL_MACHINE, value, &hKey);
		// Add the Event ID message-file name to the subkey.
		::RegSetValueExW(hKey, L"EventMessageFile", 0, REG_EXPAND_SZ, (LPBYTE)fullpath, (DWORD)((Base::Str::length(fullpath) + 1) * sizeof(wchar_t)));
		// Set the supported types flags.
		DWORD dwData = EVENTLOG_SUCCESS | EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
		::RegSetValueExW(hKey, L"TypesSupported", 0, REG_DWORD, (LPBYTE)&dwData, sizeof(dwData));
		::RegCloseKey(hKey);
	}

	Target_t get_TargetToSys(PCWSTR name, PCWSTR path)
	{
		return Target_t(new LogToSys(name, path));
	}

}
