#include <excis/sd.hpp>
#include <excis/exception.hpp>

namespace Ext {

	ustring MakeSDDL(const ustring &name, const ustring &group, mode_t mode, bool protect) {
		ustring Result;
		if (!name.empty())
			Result = Result + L"O:" + Sid(name.c_str()).as_str();
		if (!group.empty())
			Result = Result + L"G:" + Sid(group.c_str()).as_str();
		Result += L"D:";
		if (protect)
			Result += L"P";
		Result += Mode2Sddl(name, group, mode);
		return Result;
	}

}
