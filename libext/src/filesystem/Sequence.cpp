#include <libbase/std.hpp>
#include <libbase/memory.hpp>
#include <libbase/path.hpp>
#include <libbase/pcstr.hpp>
#include <libbase/logger.hpp>
#include <libext/dll.hpp>
#include <libext/filesystem.hpp>
#include <libext/exception.hpp>

namespace Fsys {

	ustring Sequence::FindStat::full_path() const
	{
		return Base::MakePath(m_path, name());
	}

	Sequence::Sequence(const ustring & path, flags_type flags) :
		m_path(path),
		m_mask(L"*"),
		m_flags(flags)
	{
	}

	Sequence::Sequence(const ustring & path, const ustring & mask, flags_type flags) :
		m_path(path),
		m_mask(mask),
		m_flags(flags)
	{
	}

	Sequence::const_iterator Sequence::begin() const
	{
		return const_iterator(*this);
	}

	Sequence::const_iterator Sequence::end() const
	{
		return const_iterator();
	}

	bool Sequence::empty() const
	{
		return begin() == end();
	}

	Sequence::const_input_iterator & Sequence::const_input_iterator::operator ++()
	{
		flags_type flags = m_impl->m_seq->flags();

		while (true) {
			FindStat & st = m_impl->m_stat;
			if (m_impl->m_find_handle == INVALID_HANDLE_VALUE) {
				st.m_path = m_impl->m_seq->path();
				ustring pattern = Base::MakePath(m_impl->m_seq->path(), m_impl->m_seq->mask());
				m_impl->m_find_handle = ::FindFirstFileW(pattern.c_str(), &st.m_stat);
				if (m_impl->m_find_handle == INVALID_HANDLE_VALUE) {
					m_impl.reset(new impl);
					break;
				}
			} else {
				if (!::FindNextFileW(m_impl->m_find_handle, &st.m_stat)) {
//					::FindClose(m_impl->m_find_handle);
					m_impl.reset(new impl);
					break;
				}
			}

			PCWSTR name = st.name();
			if (!(flags & SearchFlags_t::incDots) && (Base::Str::compare(name, L".") == 0 || Base::Str::compare(name, L"..") == 0)) {
				continue;
			}

			if ((flags & SearchFlags_t::skipSystem) && (st.attr() & FILE_ATTRIBUTE_SYSTEM)) {
				continue;
			}

			if ((flags & SearchFlags_t::skipHidden) && (st.attr() & FILE_ATTRIBUTE_HIDDEN)) {
				continue;
			}

			if ((flags & SearchFlags_t::skipDirs) && st.is_dir()) {
				continue;
			}

			if ((flags & SearchFlags_t::skipLinks) && st.is_link()) {
				continue;
			}

			if ((flags & SearchFlags_t::skipFiles) && st.is_file()) {
				continue;
			}

			break;
		}
		return *this;
	}

	Sequence::const_input_iterator Sequence::const_input_iterator::operator ++(int)
	{
		this_type ret(*this);
		operator ++();
		return ret;
	}

	const Sequence::value_type & Sequence::const_input_iterator::operator *() const
	{
		return m_impl->m_stat;
	}

	const Sequence::value_type * Sequence::const_input_iterator::operator ->() const
	{
		return &m_impl->m_stat;
	}

	bool Sequence::const_input_iterator::operator ==(const this_type & rhs) const
	{
		return m_impl->m_find_handle == rhs.m_impl->m_find_handle;
	}

	bool Sequence::const_input_iterator::operator !=(const this_type & rhs) const
	{
		return m_impl->m_find_handle != rhs.m_impl->m_find_handle;
	}

	Sequence::const_input_iterator::const_input_iterator() :
		m_impl(new impl())
	{
	}

	Sequence::const_input_iterator::const_input_iterator(const Sequence & seq) :
		m_impl(new impl(seq))
	{
		operator++();
	}

	Sequence::const_input_iterator::impl::~impl() noexcept
	{
		if (m_find_handle && m_find_handle != INVALID_HANDLE_VALUE ) {
			::FindClose(m_find_handle);
		}
	}

	Sequence::const_input_iterator::impl::impl() noexcept :
		m_seq(nullptr),
		m_find_handle(nullptr)
	{
	}

	Sequence::const_input_iterator::impl::impl(const Sequence & seq) noexcept :
		m_seq(&seq),
		m_find_handle(INVALID_HANDLE_VALUE )
	{
	}

}
