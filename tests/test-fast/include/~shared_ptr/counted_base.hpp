#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_BASE_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_BASE_HPP_

#include <basis/sys/~sync/Mutex.hpp>

namespace simstd1 {

	namespace pvt {

		template<int Num, typename Type, bool __use_ebo = !__is_final(Type) && __is_empty(Type)>
		struct ebo_helper;

		/// Specialization using EBO.
		template<int Num, typename Type>
		struct ebo_helper<Num, Type, true> : private Type
		{
			explicit ebo_helper(const Type& tp): Type(tp) {}
			static Type& get(ebo_helper& ebo) {return static_cast<Type&>(ebo);}
		};

		/// Specialization not using EBO.
		template<int Num, typename Type>
		struct ebo_helper<Num, Type, false>
		{
			explicit ebo_helper(const Type& tp): tp(tp) {}
			static Type& get(ebo_helper& eboh) {return eboh.tp;}
		private:
			Type tp;
		};

//		template<LockPolicy LockPol>
		struct choose_lock_policy
		{
			int get_use_count() const noexcept {return use_count;}
			void use_add_ref() noexcept {++use_count;}
			void weak_add_ref() noexcept {++weak_count;}

		protected:
			choose_lock_policy() noexcept: use_count(1), weak_count(1) {}
			bool use_sub_ref() noexcept {return --use_count == 0;}
			bool weak_sub_ref() noexcept {return --weak_count == 0;}

		private:
			volatile int use_count;
			volatile int weak_count;
		};

		template<LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class counted_base: public choose_lock_policy//<LockPol>
		{
		public:
			virtual ~counted_base() noexcept = default;

			counted_base() noexcept = default;

			virtual void destroy() noexcept;

			virtual void dispose() noexcept = 0;

			virtual void* get_deleter(const std::type_info&) noexcept = 0;

			void release() noexcept;

			void weak_release() noexcept;

		private:
			counted_base(const counted_base&) = delete;
			counted_base& operator =(const counted_base&) = delete;
		};

		template<LockPolicy LockPol>
		void counted_base<LockPol>::destroy() noexcept {
			delete this;
		}

		template<LockPolicy LockPol>
		void counted_base<LockPol>::release() noexcept
		{
			if (use_sub_ref())
			{
				dispose();
				if (weak_sub_ref())
					destroy();
			}
		}

		template<LockPolicy LockPol>
		void counted_base<LockPol>::weak_release() noexcept
		{
			if (weak_sub_ref())
				destroy();
		}
	}

}

#endif
