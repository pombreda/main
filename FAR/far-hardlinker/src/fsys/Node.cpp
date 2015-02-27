#include <fsys.hpp>
#include <global.hpp>

#include <basis/sys/logger.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/path.hpp>

namespace fsys {

	bool Node::is_equal_path(Node_t other) const
	{
		if (!other || cstr::compare_ci(m_name.c_str(), other->m_name.c_str()) != 0)
			return false;
		if (m_parent)
			return m_parent->is_equal_path(other->m_parent);
		return true;
	}

	ustring Node::get_full_path() const
	{
		auto ret = m_name;
		if (m_parent) {
			ustring path = m_parent->get_full_path();
			path += PATH_SEPARATOR;
			path += m_name;

			using simstd::swap;
			swap(path, ret);
		}
		return path::inplace::ensure_no_end_separator(ret);
	}

}
