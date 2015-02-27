#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/memory.hpp>

bool fsys::StatEx_i::operator ==(const fsys::StatEx_i& other) const
{
	return volume_sn() == other.volume_sn() && inode() == other.inode();
}

namespace {

	struct StatEx_impl: public fsys::StatEx_i {
		static HANDLE open(const char* path);
		static HANDLE open(const wchar_t* path);

		StatEx_impl(HANDLE hndl);

		bool is_valid() const;

		const wchar_t* name() const override;

		fsys::Size size() const override;
		fsys::Attr attr() const override;

		fsys::Time ctime() const override;
		fsys::Time atime() const override;
		fsys::Time mtime() const override;

		size_t num_links() const override;

		uint64_t volume_sn() const override;
		uint64_t inode() const override;

	private:
		BY_HANDLE_FILE_INFORMATION m_info;
	};

	HANDLE StatEx_impl::open(const char* path)
	{
//		Ext::Privilege priv(SE_BACKUP_NAME);

		return ::CreateFileA(path, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ, nullptr, OPEN_EXISTING,
//		                         FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
				                     	 0,
				                     	 nullptr);
	}

	HANDLE StatEx_impl::open(const wchar_t* path)
	{
//		Ext::Privilege priv(SE_BACKUP_NAME);

		return ::CreateFileW(path, GENERIC_READ, FILE_SHARE_DELETE | FILE_SHARE_READ, nullptr, OPEN_EXISTING,
//		                         FILE_FLAG_OPEN_REPARSE_POINT | FILE_FLAG_BACKUP_SEMANTICS,
		                     	 0,
		                     	 nullptr);
	}

	StatEx_impl::StatEx_impl(HANDLE hndl)
	{
		memory::zero(m_info);
		::GetFileInformationByHandle(hndl, &m_info);
	}

	bool StatEx_impl::is_valid() const
	{
		return m_info.nFileIndexHigh != 0 || m_info.nFileIndexLow != 0 || m_info.dwVolumeSerialNumber != 0;
	}

	const wchar_t* StatEx_impl::name() const
	{
		return EMPTY_STR;
	}

	fsys::Size StatEx_impl::size() const
	{
		return make_uint64(m_info.nFileSizeHigh, m_info.nFileSizeLow);
	}

	fsys::Attr StatEx_impl::attr() const
	{
		return m_info.dwFileAttributes;
	}

	fsys::Time StatEx_impl::ctime() const
	{
		return static_cast<fsys::Time>(make_uint64(m_info.ftCreationTime.dwHighDateTime, m_info.ftCreationTime.dwLowDateTime));
	}

	fsys::Time StatEx_impl::atime() const
	{
		return static_cast<fsys::Time>(make_uint64(m_info.ftLastAccessTime.dwHighDateTime, m_info.ftLastAccessTime.dwLowDateTime));
	}

	fsys::Time StatEx_impl::mtime() const
	{
		return static_cast<fsys::Time>(make_uint64(m_info.ftLastWriteTime.dwHighDateTime, m_info.ftLastWriteTime.dwLowDateTime));
	}

	size_t StatEx_impl::num_links() const
	{
		return m_info.nNumberOfLinks;
	}

	uint64_t StatEx_impl::volume_sn() const
	{
		return m_info.dwVolumeSerialNumber;
	}

	uint64_t StatEx_impl::inode() const
	{
		return make_uint64(m_info.nFileIndexHigh, m_info.nFileIndexLow);// & 0x0000FFFFFFFFFFFFULL;
	}

}

namespace fsys {

	StatEx stat_ex(HANDLE hndl)
	{
		if (hndl != INVALID_HANDLE_VALUE) {
			auto tmp(simstd::make_unique<StatEx_impl>(hndl));
//			simstd::unique_ptr<StatEx_impl> tmp(new StatEx_impl(hndl));
			::CloseHandle(hndl);

			if (tmp->is_valid())
				return StatEx(simstd::move(tmp));
		}
		return StatEx();
	}

	StatEx stat_ex(const char* path)
	{
		return stat_ex(StatEx_impl::open(path));
	}

	StatEx stat_ex(const wchar_t* path)
	{
		return stat_ex(StatEx_impl::open(path));
	}

}
