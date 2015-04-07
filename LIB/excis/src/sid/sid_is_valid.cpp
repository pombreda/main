#include <excis/sid.hpp>
#include <excis/exception.hpp>

namespace Ext
{
	bool Sid::is_valid(value_type in) noexcept
	{
		return in && ::IsValidSid(in);
	}
}
