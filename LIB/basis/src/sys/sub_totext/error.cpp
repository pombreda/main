﻿#include <basis/sys/totext.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/std/string>

namespace totext {

	simstd::wstring api_error(DWORD err, const wchar_t * lib)
	{
		HMODULE mod = nullptr;
		if (err && lib) {
			mod = ::LoadLibraryExW(lib, nullptr, DONT_RESOLVE_DLL_REFERENCES); //LOAD_LIBRARY_AS_DATAFILE
		}

		wchar_t * buf = nullptr;
		::FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_IGNORE_INSERTS | ((mod) ? FORMAT_MESSAGE_FROM_HMODULE : FORMAT_MESSAGE_FROM_SYSTEM), mod, err,
		                 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), //GetSystemDefaultLangID(),
		                 (wchar_t *)&buf, 0, nullptr);

		if (mod)
			::FreeLibrary(mod);

		if (!buf) {
			if (lib) {
				return api_error(err);
			} else {
				return sstr::format(L"[0x%X (%u)] Unknown error", err, err);
			}
		}

		{ // remove new line from message
			wchar_t *  cr = wcschr(buf, L'\n');
			if (cr)
			{
				if (cr > buf && *(cr - 1) == L'\r')
					*(cr - 1) = L' ';
				*cr = L' ';
			}
		}
		simstd::wstring ret = sstr::format(L"[0x%X (%u)] %s", err, err, buf);
		{ // delete return char
			ret[ret.size() - 2] = L'\0';
		}
		::LocalFree(buf);

		return simstd::move(ret);
	}

	simstd::wstring nt_status(ULONG status)
	{
		return api_error(status, L"NTDLL.DLL");
	}

	simstd::wstring wmi_error(HRESULT err)
	{
		return api_error(err, L"WMIUTILS.DLL");
	}

}
