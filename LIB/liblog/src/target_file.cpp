﻿#include <liblog/logger.hpp>

#include <libbase/lock.hpp>
#include <libbase/memory.hpp>

namespace Logger {

	struct LogToFile: public Target_i {
		~LogToFile();

		LogToFile(PCWSTR path, bool overwrite);

		void out(const Module_i * lgr, Level lvl, PCWSTR str, size_t size) const override;

		void out(PCWSTR str, size_t size) const override;

		void lock() const override;

		void unlock() const override;

	private:
		Base::auto_destroy<Base::Lock::SyncUnit_i*> m_sync;
		Base::auto_close<HANDLE> m_file;
	};

	LogToFile::~LogToFile()
	{
	}

	LogToFile::LogToFile(PCWSTR path, bool overwrite) :
		m_sync(Base::Lock::get_CritSection()),
		m_file(::CreateFileW(path, GENERIC_WRITE, FILE_SHARE_READ, nullptr, overwrite ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr))
	{
		if (m_file.is_valid()) {
			::SetFilePointer(m_file, 0, nullptr, FILE_END);
		}
	}

	void LogToFile::out(const Module_i * /*lgr*/, Level /*lvl*/, PCWSTR str, size_t size) const
	{
		out(str, size);
	}

	void LogToFile::out(PCWSTR str, size_t size) const
	{
		if (m_file.is_valid()) {
			DWORD written = 0;
			auto lk(m_sync->lock_scope());
			::WriteFile(m_file, str, size * sizeof(wchar_t), &written, nullptr);
		}
	}

	void LogToFile::lock() const
	{
		m_sync->lock();
	}

	void LogToFile::unlock() const
	{
		m_sync->release();
	}

	Target_t get_TargetToFile(PCWSTR path, bool overwrite)
	{
		return Target_t(new LogToFile(path, overwrite));
	}

}

///========================================================================================= Logging
//void logFile(WIN32_FIND_DATA info) {
//	uint64_t size = HighLow64(info.nFileSizeHigh, info.nFileSizeLow);
//	logDebug(L"%s   found: \"%s\" (Size=%I64i,%s%s%s%s%s%s%s%s%s%s%s)\n", FILE_ATTRIBUTE_DIRECTORY
//	    & info.dwFileAttributes ? L"Dir " : L"File", info.cFileName, size, FILE_ATTRIBUTE_ARCHIVE
//	    & info.dwFileAttributes ? L"ARCHIVE " : L"", FILE_ATTRIBUTE_COMPRESSED
//	    & info.dwFileAttributes ? L"COMPRESSED " : L"", FILE_ATTRIBUTE_ENCRYPTED
//	    & info.dwFileAttributes ? L"ENCRYPTED " : L"", FILE_ATTRIBUTE_HIDDEN
//	    & info.dwFileAttributes ? L"HIDDEN " : L"",
//	         FILE_ATTRIBUTE_NORMAL & info.dwFileAttributes ? L"NORMAL " : L"",
//	         FILE_ATTRIBUTE_OFFLINE & info.dwFileAttributes ? L"OFFLINE " : L"",
//	         FILE_ATTRIBUTE_READONLY & info.dwFileAttributes ? L"READONLY " : L"",
//	         FILE_ATTRIBUTE_REPARSE_POINT & info.dwFileAttributes ? L"REPARSE_POINT " : L"",
//	         FILE_ATTRIBUTE_SPARSE_FILE & info.dwFileAttributes ? L"SPARSE " : L"",
//	         FILE_ATTRIBUTE_SYSTEM & info.dwFileAttributes ? L"SYSTEM " : L"",
//	         FILE_ATTRIBUTE_TEMPORARY & info.dwFileAttributes ? L"TEMP " : L"");
//}