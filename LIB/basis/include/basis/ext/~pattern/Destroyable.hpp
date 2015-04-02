#ifndef BASIS_EXT_PATTERN_DESTROYABLE_HPP_
#define BASIS_EXT_PATTERN_DESTROYABLE_HPP_

namespace pattern
{
	struct Destroyable
	{
		virtual ~Destroyable() = default;

		virtual void destroy() const = 0;
	};
}

#endif
