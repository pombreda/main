#ifndef BASIS_MEMORY_DEFAULT_DELETERS_HPP_
#define BASIS_MEMORY_DEFAULT_DELETERS_HPP_

#include <type_traits>

namespace simstd {

	template<typename Type>
	struct default_delete
	{
		constexpr default_delete() noexcept = default;

		template<typename _Up, typename = typename std::enable_if<std::is_convertible<_Up*, Type*>::value>::type>
		default_delete(const default_delete<_Up>&) noexcept
		{
		}

		void operator ()(Type* ptr) const noexcept
		{
			delete ptr;
		}
	};

	template<>
	struct default_delete<nullptr_t>
	{
		void operator ()(nullptr_t) const noexcept
		{
		}
	};

	template<typename Type>
	struct default_delete<Type[]>
	{
		void operator ()(Type* ptr) const noexcept
		{
			delete [] ptr;
		}
	};

}

#endif
