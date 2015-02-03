#include <fsys/Sequence.hpp>

const wchar_t* fsys::Sequence::FindStat::name() const
{
	return m_stat.cFileName;
}

fsys::Size fsys::Sequence::FindStat::size() const
{
	return make_uint64(m_stat.nFileSizeHigh, m_stat.nFileSizeLow);
}

fsys::Attr fsys::Sequence::FindStat::attr() const
{
	return m_stat.dwFileAttributes;
}

fsys::Time fsys::Sequence::FindStat::ctime() const
{
	return static_cast<Time>(make_uint64(m_stat.ftCreationTime.dwHighDateTime, m_stat.ftCreationTime.dwLowDateTime));
}

fsys::Time fsys::Sequence::FindStat::atime() const
{
	return static_cast<Time>(make_uint64(m_stat.ftLastAccessTime.dwHighDateTime, m_stat.ftLastAccessTime.dwLowDateTime));
}

fsys::Time fsys::Sequence::FindStat::mtime() const
{
	return static_cast<Time>(make_uint64(m_stat.ftLastWriteTime.dwHighDateTime, m_stat.ftLastWriteTime.dwLowDateTime));
}
