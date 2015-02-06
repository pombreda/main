#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>
#include <basis/sys/memory.hpp>
#include <basis/sys/totext.hpp>

#include <fsys.hpp>

namespace fsys {

	Sequence::Sequence(const ustring& path, const ustring& mask, const Options& options) :
		m_path(path),
		m_mask(mask),
		m_options(options)
	{
		LogTraceObj();
		LogDebug(L"path:                '%s'\n", m_path.c_str());
		LogDebug(L"options.mask:        '%s'\n", m_mask.c_str());
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

	const Sequence::Options& Sequence::options() const
	{
		return m_options;
	}

	///=================================================================================================================
	Sequence::ci_iterator & Sequence::ci_iterator::operator ++()
	{
		while (true) {
			auto& st = m_impl->findStat;
			auto& fh = m_impl->findHandle;
			if (fh == INVALID_HANDLE_VALUE) {
				auto pattern = path::make(m_impl->sequence->path(), m_impl->sequence->mask());
				fh = ::FindFirstFileW(pattern.c_str(), &st.m_stat);
				LogErrorIf(fh == INVALID_HANDLE_VALUE, L"'%s' -> %s\n", pattern.c_str(), totext::api_error().c_str());
			} else {
				if (!::FindNextFileW(fh, &st.m_stat)) {
					::FindClose(fh);
					fh = INVALID_HANDLE_VALUE;
				}
			}

			if (fh == INVALID_HANDLE_VALUE) {
				m_impl.reset(new impl);
				break;
			}

			auto& options = m_impl->sequence->options();
			if (!options.apply_filters(st))
				break;
		}
		return *this;
	}

	Sequence::ci_iterator Sequence::ci_iterator::operator ++(int)
	{
		this_type ret(*this);
		operator ++();
		return ret;
	}

	const Sequence::value_type & Sequence::ci_iterator::operator *() const
	{
		return m_impl->findStat;
	}

	const Sequence::value_type * Sequence::ci_iterator::operator ->() const
	{
		return &m_impl->findStat;
	}

	bool Sequence::ci_iterator::operator ==(const this_type& rhs) const
	{
		return m_impl->findHandle == rhs.m_impl->findHandle;
	}

	bool Sequence::ci_iterator::operator !=(const this_type& rhs) const
	{
		return m_impl->findHandle != rhs.m_impl->findHandle;
	}

	Sequence::ci_iterator::ci_iterator() :
		m_impl(new impl)
	{
	}

	Sequence::ci_iterator::ci_iterator(const Sequence& seq) :
		m_impl(new impl(seq))
	{
		operator++();
	}

	///=================================================================================================================
	Sequence::ci_iterator::impl::~impl() noexcept
	{
		LogTraceObj();
		if (findHandle && findHandle != INVALID_HANDLE_VALUE ) {
			::FindClose(findHandle);
		}
	}

	Sequence::ci_iterator::impl::impl() noexcept :
		sequence(),
		findHandle()
	{
		LogTraceObj();
	}

	Sequence::ci_iterator::impl::impl(const Sequence& sequence) noexcept :
		sequence(&sequence),
		findHandle(INVALID_HANDLE_VALUE)
	{
		LogTraceObj();
	}

}
