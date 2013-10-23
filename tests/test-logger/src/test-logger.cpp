#include <liblog/logger.hpp>
#include <libbase/std.hpp>
#include <libbase/atexit.hpp>
#include <libbase/console.hpp>

#include <memory>

namespace {
	void setup_logger()
	{
		using namespace Logger;
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		set_default_level(Level::Trace);
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		set_default_prefix(Prefix::Full/* | Prefix::Place*/);
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
//		set_default_target(get_TargetToConsole());
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	}
}

class A {
public:
	~A()
	{
		Base::Console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	A() :
		m_int(1)
	{
		Base::Console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	int get_int() const
	{
		return m_int;
	}

private:
	int m_int;
};

class B: public A {
public:
	~B()
	{
		Base::Console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	B() :
		m_double(10)
	{
		Base::Console::printf(L"%S\n", __PRETTY_FUNCTION__);
	}

	double get_double() const
	{
		return m_double;
	}

private:
	double m_double;
};

#ifdef NDEBUG
int wWmain()
{
#else
	int main() {
#endif
//	Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	setup_logger();

//	Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
	LogTrace();

//	Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	{
		std::shared_ptr<A> sptr(std::make_shared<B>());
	}

	LogTrace();
	return 0;
}

/// ========================================================================== Startup (entry point)
#ifdef NDEBUG
extern "C" {
	int atexit(Base::CrtFunction pf)
	{
		return Base::atexit(pf);
	}

	void __cxa_pure_virtual(void)
	{
		Base::cxa_pure_virtual();
	}

	int mainCRTStartup()
	{
//	int	WinMainCRTStartup() {
		Base::init_atexit();
//		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		int Result = 0;
//		STARTUPINFO StartupInfo = {sizeof(STARTUPINFO), 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//		::GetStartupInfo(&StartupInfo);
//
//		Result = wWinMain(::GetModuleHandle(nullptr), nullptr, ::GetCommandLine(),
//						  StartupInfo.dwFlags & STARTF_USESHOWWINDOW ? StartupInfo.wShowWindow : SW_SHOWDEFAULT);
		Result = wWmain();
		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		Base::invoke_atexit();
		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		::ExitProcess(Result);
		Base::Console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);
		return Result;
	}

//	BOOL WINAPI	DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID) {
//		switch (dwReason) {
//			case DLL_PROCESS_ATTACH:
//				init_atexit();
//				break;
//
//			case DLL_PROCESS_DETACH:
//				invoke_atexit();
//				break;
//		}
//		return true;
//	}
}
#endif