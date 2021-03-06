#include <basis/os/dbghelp.hpp>
#include <basis/sys/traceback.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/sstr.hpp>
#include <basis/sys/totext.hpp>
#include <basis/ext/pattern.hpp>

#include <basis/simstd/string>

#include "Bfd.hpp"

namespace traceback
{
	LogRegisterLocal(L"traceback");

	struct FrameImpl:
		public Frame_i,
		private pattern::Uncopyable
	{
		~FrameImpl();

		FrameImpl(void* address);

		void* address() const override;

		const ustring& module() const override;

		const ustring& file() const override;

		const ustring& function() const override;

		size_t line() const override;

		size_t offset() const override;

		ustring to_str() const override;

	private:
		bool LoadFromPDB(void* frame);

		bool LoadFromBfd(void* frame);

		bool LoadFromMap(void* frame);

		void* m_address;
		uint64_t m_module_base;
		ustring m_module;
		ustring m_image;

		ustring m_file;
		ustring m_function;
		size_t  m_line;
		size_t  m_offset;
	};

	FrameImpl::~FrameImpl()
	{
		TraceFunc();
	}

	FrameImpl::FrameImpl(void* address)
		: m_address()
		, m_module_base()
		, m_line()
		, m_offset()
	{
		TraceFunc();
		IMAGEHLP_MODULEW64 modInfo;
		memory::zero(modInfo);
		modInfo.SizeOfStruct = sizeof(modInfo) - 8;

		LogTrace(L"[%p]\n", address);
		bool res = os::Dbghelp_dll::inst().SymGetModuleInfoW64(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &modInfo);

		if (res) {
			m_module = modInfo.ModuleName;
			m_module_base = modInfo.BaseOfImage;
			m_image = modInfo.ImageName;
			LogTrace(L"BaseOfImage:     0x%I64X\n", modInfo.BaseOfImage);
			LogTrace(L"ImageSize:       %u\n", modInfo.ImageSize);
			LogTrace(L"TimeDateStamp:   %u\n", modInfo.TimeDateStamp);
			LogTrace(L"CheckSum:        %u\n", modInfo.CheckSum);
			LogTrace(L"NumSyms:         %u\n", modInfo.NumSyms);
			LogTrace(L"SymType:         %d\n", (int)modInfo.SymType);
			LogTrace(L"ModuleName:      '%s'\n", modInfo.ModuleName);
			LogTrace(L"ImageName:       '%s'\n", modInfo.ImageName);
			LogTrace(L"LoadedImageName: '%s'\n", modInfo.LoadedImageName);
			LogTrace(L"LoadedPdbName:   '%s'\n", modInfo.LoadedPdbName);
		} else {
			LogError(L"%s %p\n", totext::api_error().c_str(), address);
		}

		(modInfo.SymType && LoadFromPDB(address)) || LoadFromBfd(address) || LoadFromMap(address);
		TraceFunc();
	}

	void * FrameImpl::address() const
	{
		return m_address;
	}

	const ustring& FrameImpl::module() const
	{
		return m_module;
	}

	const ustring& FrameImpl::file() const
	{
		return m_file;
	}

	const ustring& FrameImpl::function() const
	{
		return m_function;
	}

	size_t FrameImpl::line() const
	{
		return m_line;
	}

	size_t FrameImpl::offset() const
	{
		return m_offset;
	}

	ustring FrameImpl::to_str() const
	{
		return ustring();
	}

	bool FrameImpl::LoadFromPDB(void* address)
	{
		TraceFunc();
		LogTraceLn();
		bool ret = false;

		{
			size_t size = sizeof(SYMBOL_INFOW) + MAX_SYM_NAME * sizeof(wchar_t);
			SYMBOL_INFOW* symInfo = static_cast<SYMBOL_INFOW*>(HostAlloc(memory::heap::DefaultStat, size));
			memory::zero(symInfo, sizeof(*symInfo));
			symInfo->SizeOfStruct = sizeof(*symInfo);
			symInfo->MaxNameLen = MAX_SYM_NAME;

			DWORD64 displacement;
			bool res = os::Dbghelp_dll::inst().SymFromAddrW(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &displacement, symInfo);
			LogWarnIf(!res, L"%s\n", totext::api_error().c_str());
			if (res) {
				m_address = reinterpret_cast<void*>(symInfo->Address);
				m_function = symInfo->Name;
				m_offset = (size_t)displacement;
				ret = true;
			}
			HostFree(memory::heap::DefaultStat, symInfo);
		}

		{
			IMAGEHLP_LINEW64 info;
			memory::zero(info);
			info.SizeOfStruct = sizeof(info);

			DWORD dwLineDisplacement = 0;
			bool res = os::Dbghelp_dll::inst().SymGetLineFromAddrW64(::GetCurrentProcess(), reinterpret_cast<DWORD64>(address), &dwLineDisplacement, &info);
//			LogErrorIf(!res, L"%s\n", totext::api_error().c_str());
			if (res) {
				m_line = info.LineNumber;
				m_file = info.FileName;
			}
		}
		TraceFunc();
		return ret;
	}

	bool FrameImpl::LoadFromBfd(void* address)
	{
		TraceFunc();
		LogTraceLn();
		bool ret = false;
		m_address = address;
#if defined(__GNUC__) && defined(DEBUG)
		const char* file = nullptr;
		const char* func = nullptr;
		bfd_fill(file, func, m_line, m_image.c_str(), m_address);
		LogTrace(L"file: '%S'\n", file);
		LogTrace(L"func: '%S'\n", func);
		LogTrace(L"line: %Id\n", m_line);
		LogTrace(L"offs: %Id\n", m_offset);

		if (file)
			m_file = sstr::cp2w(filename_only(file), CP_OEMCP);
		if (func) {
			char buf[MAX_PATH];
			size_t size = sizeof(buf);
			int st = 0;
			abi::__cxa_demangle(func, buf, &size, &st);
			m_function = sstr::cp2w(st ? func : buf, CP_OEMCP);
		}
		ret = file || func;
#endif
		TraceFunc();
		return ret;
	}

	bool FrameImpl::LoadFromMap(void* address)
	{
		UNUSED(address);
		TraceFunc();
		TraceFunc();
		return false;
	}

	Frame read_frame_data(void* address)
	{
		return simstd::make_unique<FrameImpl>(address);
	}
}
