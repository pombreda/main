#ifndef BASIS_EXT_PATTERN_COMMAND_HPP_
#define BASIS_EXT_PATTERN_COMMAND_HPP_

namespace pattern
{
	struct Command
	{
		virtual ~Command() = default;

		virtual ssize_t execute() = 0;
	};
}

#endif
