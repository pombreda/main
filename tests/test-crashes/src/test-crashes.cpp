#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/totext.hpp>
#include <basis/sys/traceback.hpp>

#include <excis/exception.hpp>

#include <basis/simstd/string>

namespace {
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=re;prefix=fu;target=co");

		traceback::init();
	}

	void do_av_exception()
	{
		LogWarn(L"accessing null pointer\n");
		volatile int* p = nullptr;
		*p = 0;
	}
}

extern "C" int wmain(int argc, wchar_t * argv[])
{
	UNUSED(argc);
	UNUSED(argv);

	console::printf(L"%S:%d\n", __PRETTY_FUNCTION__, __LINE__);

	setup_logger();

	exception::set_vectored_exception_filter();
	exception::set_unhandled_exception_filter();

	try {

		LogTrace();

		do_av_exception();

	} catch (exception::AbstractError & e) {
		LogFatal(L"SEH exception cought: %s\n", e.what().c_str());
		LogFatal(L"SEH exception cought: %s\n", e.where().c_str());

		auto mstr = e.format_error();
		for (size_t i = 0; i < mstr.size(); ++i)
			LogFatal(L"\t%s\n", mstr[i]);
	} catch (...) {
		LogFatal(L"cpp exception cought\n");
	}

	return 0;
}

int WINAPI wWinMain(HINSTANCE /*hInstance*/, HINSTANCE /*hPrevInstance*/, LPWSTR /*lpCmdLine*/, int /*nShowCmd*/)
{
	return wmain(0, nullptr);
}
