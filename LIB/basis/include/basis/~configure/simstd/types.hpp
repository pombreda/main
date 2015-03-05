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

namespace simstd
{
	template<bool Condition, typename IfTrue, typename IfFalse>
	struct conditional;

	template<typename IfTrue, typename IfFalse>
	struct conditional<true, IfTrue, IfFalse>
	{
		using type = IfTrue;
	};

	template<typename IfTrue, typename IfFalse>
	struct conditional<false, IfTrue, IfFalse>
	{
		using type = IfFalse;
	};

	template<typename...>
	struct _or_;

	template<>
	struct _or_<> : public false_type
	{
	};

	template<typename Type>
	struct _or_<Type> : public Type
	{
	};

	template<typename Type1, typename Type2>
	struct _or_<Type1, Type2> : public conditional<Type1::value, Type1, Type2>::type
	{
	};

	template<typename Type1, typename Type2, typename Type3, typename ... TypeN>
	struct _or_<Type1, Type2, Type3, TypeN...> : public conditional<Type1::value, Type1, _or_ <Type2, Type3, TypeN...>>::type
	{
	};

	template<typename ...>
	struct _and_;

	template<>
	struct _and_<> : public true_type
	{
	};

	template<typename Type1>
	struct _and_<Type1> : public Type1
	{
	};

	template<typename Type1, typename Type2>
	struct _and_<Type1, Type2> : public conditional<Type1::value, Type2, Type1>::type
	{
	};

	template<typename Type1, typename Type2, typename Type3, typename ... TypeN>
	struct _and_<Type1, Type2, Type3, TypeN...> : public conditional<Type1::value, _and_ <Type2, Type3, TypeN...>, Type1>::type
	{
	};

	template<typename Type>
	struct _not_: public integral_constant<bool, !Type::value>
	{
	};
}

namespace simstd
{
	namespace pvt
	{
		template<typename>
		struct is_void : public false_type {};

		template<>
		struct is_void<void> : public true_type {};
	}

	template<typename Type>
	struct is_void : public pvt::is_void<remove_cv_t<Type>>::type
	{
	};

	template<typename>
	struct is_pointer : public false_type {};

	template<typename Type>
	struct is_pointer<Type*> : public true_type {};

	template<typename>
	struct is_lvalue_reference : public false_type {};

	template<typename Type>
	struct is_lvalue_reference<Type&> : public true_type {};

	template<typename>
	struct is_rvalue_reference : public false_type {};

	template<typename Type>
	struct is_rvalue_reference<Type&&> : public true_type {};

	template<typename Type>
	struct is_reference : public _or_<is_lvalue_reference<Type>, is_rvalue_reference<Type>>::type
	{
	};

}

namespace simstd
{
	namespace pvt
	{
		template<typename Type>
		struct alignment_of : public integral_constant<size_t, __alignof__(Type)>
		{
		};

//		template<size_t Length>
//		struct aligned_storage_msa
//		{
//			union type
//			{
//				unsigned char data[Length];
//				struct __attribute__((__aligned__)) {} __align;
//			};
//		};

//		template<size_t Length, size_t Align = __alignof__(typename aligned_storage_msa<Length>::type)>
		template<size_t Length, size_t Align>
		struct aligned_storage
		{
			union type
			{
				unsigned char data[Length];
				struct __attribute__((__aligned__((Align)))) {} align;
			};
		};

		template<typename Type>
		struct aligned_buffer : aligned_storage<sizeof(Type), alignment_of<Type>::value>
		{
			void* addr() noexcept
			{
				return static_cast<void*>(&storage);
			}

			const void* addr() const noexcept
			{
				return static_cast<const void*>(&storage);
			}

			Type* ptr() noexcept
			{
				return static_cast<Type*>(addr());
			}

			const Type* ptr() const noexcept
			{
				return static_cast<const Type*>(addr());
			}

			typename aligned_storage<sizeof(Type), alignment_of<Type>::value>::type storage;
		};
	}
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
