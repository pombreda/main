#ifndef BASIS_CONFIGURE_SIMSTD_TYPES_HPP_
#define BASIS_CONFIGURE_SIMSTD_TYPES_HPP_

namespace simstd {

	typedef decltype(nullptr) nullptr_t;

	template<typename Type>
	Type* addressof(Type& ref) noexcept
	{
		return reinterpret_cast<Type*>(&const_cast<char&>(reinterpret_cast<const volatile char&>(ref)));
	}

	template<typename Type>
	struct remove_reference {
		typedef Type type;
	};

	template<typename Type>
	struct remove_reference<Type&> {
		typedef Type type;
	};

	template<typename Type>
	struct remove_reference<Type&&> {
		typedef Type type;
	};

#if defined(__GNUC__)
	template<typename Type>
	constexpr Type&& forward(typename remove_reference<Type>::type& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	constexpr Type&& forward(typename remove_reference<Type>::type&& val) noexcept
	{
		return static_cast<Type&&>(val);
	}

	template<typename Type>
	constexpr typename remove_reference<Type>::type&& move(Type&& val) noexcept
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
			using DeleterType = typename simstd::remove_reference<Deleter>::type;

			template<typename TypeI>
			static typename TypeI::pointer recognize_pointer(typename TypeI::pointer*);

			template<typename TypeI>
			static Type* recognize_pointer(...);

		public:
			typedef decltype(recognize_pointer<DeleterType>(0)) type;
		};

	}

}

namespace simstd {
//#ifdef __x86_64__
//	typedef long long unsigned int size_t;
//	typedef long long int ssize_t;
//	typedef long long int ptrdiff_t;
//#else
//	typedef unsigned int size_t;
//	typedef int ssize_t;
//	typedef int ptrdiff_t;
//#endif

	namespace Test {
		typedef int (*aPrintFunc)(const char *, ...);
		typedef int (*wPrintFunc)(const wchar_t *, ...);
	}
}

#endif
