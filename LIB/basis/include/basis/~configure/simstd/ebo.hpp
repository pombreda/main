#ifndef BASIS_CONFIGURE_SIMSTD_EBO_HPP_
#define BASIS_CONFIGURE_SIMSTD_EBO_HPP_

namespace simstd
{
	namespace pvt
	{
		/// Empty base optimization
		template<int Num, typename Type, bool use_ebo = !__is_final(Type) && __is_empty(Type)>
		struct ebo_helper;

		/// Specialization using EBO
		template<int Num, typename Type>
		struct ebo_helper<Num, Type, true> : private Type
		{
			explicit ebo_helper(const Type& other): Type(other) {}

			ebo_helper(Type&& other): Type(simstd::move(other)) {}

			void swap(ebo_helper& other) noexcept {using simstd::swap; swap(static_cast<Type&>(*this), static_cast<Type&>(other));}

			static Type& get(ebo_helper& ebo) {return static_cast<Type&>(ebo);}
			static const Type& get(const ebo_helper& ebo) {return static_cast<Type&>(ebo);}
		};

		/// Specialization not using EBO
		template<int Num, typename Type>
		struct ebo_helper<Num, Type, false>
		{
			explicit ebo_helper(const Type& other): value(other) {}

			ebo_helper(Type&& other): value(simstd::move(other)) {}

			void swap(ebo_helper& other) noexcept {using simstd::swap; swap(value, other.value);}

			static Type& get(ebo_helper& ebo) {return ebo.value;}
			static const Type& get(const ebo_helper& ebo) {return ebo.value;}

		private:
			Type value;
		};

	}

	template<int Num, typename Type, bool use_ebo>
	void swap(pvt::ebo_helper<Num, Type, use_ebo>& a, pvt::ebo_helper<Num, Type, use_ebo>& b) noexcept
	{
		a.swap(b);
	}
}

#endif
