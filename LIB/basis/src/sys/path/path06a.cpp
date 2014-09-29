#include <basis/sys/path.hpp>

#include <basis/simstd/string>

namespace path {

	ustring make(const ustring & path, const ustring &name)
	{
		return make(path.c_str(), name.c_str());
	}

}