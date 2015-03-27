#include <basis/sys/memory.hpp>
#include <basis/sys/logger.hpp>
#include <excis/filesystem.hpp>
#include <excis/exception.hpp>

namespace fsys {
	Stat::Stat(PCWSTR path)
	{
		memory::auto_close<HANDLE> hndl(HandleRead(path));
		refresh(hndl);
	}

	void Stat::refresh(HANDLE hndl)
	{
		CheckApi(::GetFileInformationByHandle(hndl, this));
//		CheckApi(::GetFileInformationByHandleEx(hndl, FileBasicInfo, &m_basic_info, sizeof(m_basic_info)));
//		CheckApi(::GetFileInformationByHandleEx(hndl, FileStandardInfo, &m_standard_info, sizeof(m_standard_info)));
//		::GetFileInformationByHandleEx(hndl, (FILE_INFO_BY_HANDLE_CLASS)0x12, &m_id_info, sizeof(m_id_info));
//		LogTraceLn();
//		LogTrace(L"CreationTime:   %I64d\n", m_basic_info.CreationTime.QuadPart);
//		LogTrace(L"LastAccessTime: %I64d\n", m_basic_info.LastAccessTime.QuadPart);
//		LogTrace(L"LastWriteTime:  %I64d\n", m_basic_info.LastWriteTime.QuadPart);
//		LogTrace(L"ChangeTime:     %I64d\n", m_basic_info.ChangeTime.QuadPart);
//		LogTrace(L"FileAttributes: 0x%X\n", m_basic_info.FileAttributes);
//		LogTrace(L"AllocationSize: %I64d\n", m_standard_info.AllocationSize.QuadPart);
//		LogTrace(L"EndOfFile:      %I64d\n", m_standard_info.EndOfFile.QuadPart);
//		LogTrace(L"NumberOfLinks:  %u\n", m_standard_info.NumberOfLinks);
//		LogTrace(L"DeletePending:  %d\n", (int)m_standard_info.DeletePending);
//		LogTrace(L"Directory:      %d\n", (int)m_standard_info.Directory);
//		LogTrace(L"VolumeSNumber:  0x%I64X\n", m_id_info.VolumeSerialNumber);
//		LogTrace(L"FileId:         0x%I64X 0x%I64X\n", m_id_info.FileId.HighPart, m_id_info.FileId.LowPart);
	}

	Stat & Stat::operator =(HANDLE hndl)
	{
		refresh(hndl);
		return *this;
	}

	Stat & Stat::operator =(PCWSTR path)
	{
		memory::auto_close<HANDLE> hndl(HandleRead(path));
		refresh(hndl);
		return *this;
	}
}
