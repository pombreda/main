#include <excis/sid.hpp>
#include <excis/exception.hpp>

namespace Ext
{
	PSID Sid::get_sid(WELL_KNOWN_SID_TYPE wns)
	{
		DWORD size = SECURITY_MAX_SID_SIZE;
		auto ret = static_cast<value_type>(::LocalAlloc(LPTR, size));
		CheckApi(::CreateWellKnownSid(wns, nullptr, ret, &size));
		return ret;
	}
}
