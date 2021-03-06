#include <basis/sys/path.hpp>
#include <basis/sys/cstr.hpp>
#include <basis/sys/sstr.hpp>

#include <basis/simstd/string>

namespace path {

	namespace inplace {

		ustring& ensure_no_end_separator(ustring& path)
		{
			if (!path.empty() && cstr::find(PATH_SEPARATORS, path[path.size() - 1]))
				path.pop_back();
			return path;
		}

	}

}
