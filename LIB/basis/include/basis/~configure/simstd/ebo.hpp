#ifndef BASIS_CONFIGURE_SIMSTD_EBO_HPP_
#define BASIS_CONFIGURE_SIMSTD_EBO_HPP_

namespace simstd {
	namespace pvt {

		/// Empty base optimization
		template<int Num, typename Type, bool use_ebo = !__is_final(Type) && __is_empty(Type)>
		struct ebo_helper;

		/// Specialization using EBO.
		template<int Num, typename Type>
		struct ebo_helper<Num, Type, true> : private Type
		{
			explicit ebo_helper(const Type& tp): Type(tp) {}
			ebo_helper(Type&& tp): Type(simstd::move(tp)) {}
			void swap(ebo_helper&) noexcept {}
			static Type& get(ebo_helper& ebo) {return static_cast<Type&>(ebo);}
			static const Type& get(const ebo_helper& ebo) {return static_cast<Type&>(ebo);}
		};

		/// Specialization not using EBO.
		template<int Num, typename Type>
		struct ebo_helper<Num, Type, false>
		{
			explicit ebo_helper(const Type& tp): tp(tp) {}
			ebo_helper(Type&& tp): tp(simstd::move(tp)) {}
			void swap(ebo_helper& other) noexcept {using simstd::swap; swap(tp, other.tp);}
			static Type& get(ebo_helper& ebo) {return ebo.tp;}
			static const Type& get(const ebo_helper& ebo) {return ebo.tp;}
		private:
			Type tp;
		};

	}

	template<int Num, typename Type, bool use_ebo>
	void swap(pvt::ebo_helper<Num, Type, use_ebo>& ebo1, pvt::ebo_helper<Num, Type, use_ebo>& ebo2) noexcept
	{
		ebo1.swap(ebo2);
	}

}

#endif
