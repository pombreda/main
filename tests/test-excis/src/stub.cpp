﻿/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
#include <basis/sys/console.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/linkage.hpp>
#include <basis/sys/memory.hpp>

extern "C" int wmain(int argc, wchar_t* argv[]);

extern int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int);

namespace {

	void prolog()
	{
		TraceFunc();
		crt::init_atexit();
		TraceFunc();
	}

	int epilog(int errcode)
	{
		TraceFunc();
		crt::invoke_atexit();

		TraceFunc();
		crt::check_default_heap();

		TraceFunc();

		::ExitProcess(errcode);
		return errcode;
	}

}

extern "C" {

	LONG WINAPI RedirectedSetUnhandledExceptionFilter(EXCEPTION_POINTERS * /*ExceptionInfo*/)
	{
		// When the CRT calls SetUnhandledExceptionFilter with NULL parameter
		// our handler will not get removed.

		console::printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

		return 0;
	}

	int mainCRTStartup()
	{
		TraceFunc();

		prolog();
//		linkage::CAPIHook apiHook1("kernel32.dll", "SetUnhandledExceptionFilter", (PROC)RedirectedSetUnhandledExceptionFilter);

		int argc = 0;
		wchar_t ** argv = ::CommandLineToArgvW(::GetCommandLineW(), &argc);

		int ret = wmain(argc, argv);

		::LocalFree(argv);

		TraceFunc();
		return epilog(ret);
	}

	int	WinMainCRTStartup() // -mwindows
	{
		TraceFunc();

		prolog();

		STARTUPINFOW startupInfo;
		::RtlSecureZeroMemory(&startupInfo, sizeof(startupInfo));
		startupInfo.cb = sizeof(startupInfo);
		::GetStartupInfoW(&startupInfo);

		int ret = wWinMain(::GetModuleHandleW(nullptr), nullptr, ::GetCommandLineW(), startupInfo.dwFlags & STARTF_USESHOWWINDOW ? startupInfo.wShowWindow : SW_SHOWDEFAULT);

		TraceFunc();
		return epilog(ret);
	}

//	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID)
//	{
//		console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		switch (dwReason) {
//			case DLL_PROCESS_ATTACH:
//				crt::init_atexit();
//				break;
//
//			case DLL_PROCESS_DETACH:
//				crt::invoke_atexit();
//				break;
//		}
//		return true;
//	}

	int atexit(crt::Function pf)
	{
		TraceFormatFunc("%s:%d func: %p\n", __PRETTY_FUNCTION__, __LINE__, pf);
		return crt::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		TraceFunc();
		crt::cxa_pure_virtual();
	}

}
#endif
