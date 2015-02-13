#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_BASE_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_BASE_HPP_

#include <basis/sys/~sync/Mutex.hpp>

namespace simstd1 {

	namespace pvt {

//		template<LockPolicy LockPol>
		struct choose_lock_policy
		{
			ssize_t get_use_count() const noexcept {return use_count;}
			bool use_add_ref() noexcept {return use_count ? ++use_count, true : false;}
			bool weak_add_ref() noexcept {return weak_count ? ++weak_count, true : false;}

		protected:
			choose_lock_policy() noexcept: use_count(1), weak_count(1) {}
			bool use_sub_ref() noexcept {return --use_count == 0;}
			bool weak_sub_ref() noexcept {return --weak_count == 0;}

		private:
			volatile ssize_t use_count;
			volatile ssize_t weak_count;
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
