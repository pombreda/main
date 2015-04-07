#include <excis/sid.hpp>
#include <excis/exception.hpp>

namespace Ext
{
	Sid::size_type Sid::size(value_type in)
	{
		return check(in), ::GetLengthSid(in);
	}
}
