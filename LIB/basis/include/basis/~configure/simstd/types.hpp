#ifndef BASIS_CONFIGURE_SIMSTD_TYPES_HPP_
#define BASIS_CONFIGURE_SIMSTD_TYPES_HPP_

namespace simstd {

	typedef decltype(nullptr) nullptr_t;

	struct nothrow_t
	{
	};

	const nothrow_t nothrow;

	template<typename Type>
	Type* addressof(Type& ref) noexcept
	{
		return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(ref)));
	}

	template<typename Type>
	struct remove_const
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_const<Type const>
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_volatile
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_volatile<Type volatile>
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_cv
	{
		using type = typename remove_const<typename remove_volatile<Type>::type>::type;
	};

	template<typename Type>
	struct add_const
	{
		using type = const Type;
	};

	template<typename Type>
	struct add_volatile
	{
		using type = volatile Type;
	};

	template<typename Type>
	struct add_cv
	{
		using type = typename add_const<typename add_volatile<Type>::type>::type;
	};

	template<typename Type>
	using remove_const_t = typename remove_const<Type>::type;

	template<typename Type>
	using remove_volatile_t = typename remove_volatile<Type>::type;

	template<typename Type>
	using remove_cv_t = typename remove_cv<Type>::type;

	template<typename Type>
	using add_const_t = typename add_const<Type>::type;

	template<typename Type>
	using add_volatile_t = typename add_volatile<Type>::type;

	template<typename Type>
	using add_cv_t = typename add_cv<Type>::type;

	template<typename Type>
	struct remove_reference
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_reference<Type&>
	{
		using type = Type;
	};

	template<typename Type>
	struct remove_reference<Type&&>
	{
		using type = Type;
	};

	template<typename Type>
	using remove_reference_t = typename remove_reference<Type>::type;

#if defined(__GNUC__)
	template<typename Type>
	constexpr Type&& forward(remove_reference_t<Type>& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	constexpr Type&& forward(remove_reference_t<Type>&& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	constexpr remove_reference_t<Type>&& move(Type&& val) noexcept
	{
		return static_cast<typename remove_reference<Type>::type&&>(val);
	}
#else
	template<typename Type>
	Type&& forward(typename remove_reference<Type>::type& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	Type&& forward(typename remove_reference<Type>::type&& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	typename remove_reference<Type>::type&& move(Type&& val) noexcept
	{
		return static_cast<typename remove_reference<Type>::type&&>(val);
	}
#endif

	namespace pvt {

		template<typename Type, typename Deleter>
		class RecognizePointer {
			template<typename TypeI>
			static typename TypeI::pointer recognize_pointer(typename TypeI::pointer*);

			template<typename TypeI>
			static Type* recognize_pointer(...);

		public:
			typedef decltype(recognize_pointer<remove_reference_t<Deleter>>(0)) type;
		};

	}

}

namespace simstd {

	template<typename Type, Type Val>
	struct integral_constant
	{
		static constexpr Type value = Val;
		typedef Type value_type;
		typedef integral_constant<Type, Val> type;
		constexpr operator value_type()
		{
			return value;
		}
	};

	template<typename Type, Type Val>
	constexpr Type integral_constant<Type, Val>::value;

	typedef integral_constant<bool, true> true_type;

	typedef integral_constant<bool, false> false_type;

}

//#ifdef __x86_64__
//	typedef long long unsigned int size_t;
//	typedef long long int ssize_t;
//	typedef long long int ptrdiff_t;
//#else
//	typedef unsigned int size_t;
//	typedef int ssize_t;
//	typedef int ptrdiff_t;
//#endif

///=============================================================================== Special placement
inline void* operator new(size_t, void* p, const simstd::nothrow_t&) throw()
{
	return p;
}

inline void* operator new[](size_t, void* p, const simstd::nothrow_t&) throw()
{
	return p;
}

#endif
