#ifndef BASIS_MEMORY_SHARED_PTR_COUNTED_BASE_HPP_
#define BASIS_MEMORY_SHARED_PTR_COUNTED_BASE_HPP_

#include <basis/sys/~sync/Mutex.hpp>

namespace simstd {
	namespace pvt {

		template<LockPolicy LockPol>
		struct choose_lock_policy
		{
			ssize_t get_use_count() const noexcept {return use_count;}
			bool add_use_ref_count() noexcept {return use_count != 0 ? ++use_count, true : false;}
			bool add_weak_ref_count() noexcept {return weak_count != 0 ? ++weak_count, true : false;}

		protected:
			choose_lock_policy(ssize_t use_count, ssize_t weak_count) noexcept: use_count(use_count), weak_count(weak_count) {}
			bool sub_use_ref_count() noexcept {return --use_count != 0;}
			bool sub_weak_ref_count() noexcept {return --weak_count != 0;}

		private:
			volatile ssize_t use_count;
			volatile ssize_t weak_count;
		};

		template<>
		struct choose_lock_policy<LockPolicy::ATOMIC>
		{
			ssize_t get_use_count() const noexcept {return use_count;}
			bool add_use_ref_count() noexcept {return use_count != 0 ? ++use_count, true : false;}
			bool add_weak_ref_count() noexcept {return weak_count != 0 ? ++weak_count, true : false;}

		protected:
			choose_lock_policy(ssize_t use_count, ssize_t weak_count) noexcept: use_count(use_count), weak_count(weak_count) {}
			bool sub_use_ref_count() noexcept {return --use_count != 0;}
			bool sub_weak_ref_count() noexcept {return --weak_count != 0;}

		private:
			atomic_int use_count;
			atomic_int weak_count;
		};

		template<>
		struct choose_lock_policy<LockPolicy::MUTEX>
		{
			ssize_t get_use_count() const noexcept {return use_count;}
			bool add_use_ref_count() noexcept {return use_count != 0 ? increase_use(), true : false;}
			bool add_weak_ref_count() noexcept {return weak_count != 0 ? increase_weak(), true : false;}

		protected:
			choose_lock_policy(ssize_t use_count, ssize_t weak_count) noexcept: use_count(use_count), weak_count(weak_count) {}
			bool sub_use_ref_count() noexcept {return decrease_use() != 0;}
			bool sub_weak_ref_count() noexcept {return decrease_weak() != 0;}

		private:
			ssize_t increase_use() {mutex.lock(); ssize_t ret = ++use_count; mutex.unlock(); return ret;}
			ssize_t decrease_use() {mutex.lock(); ssize_t ret = --use_count; mutex.unlock(); return ret;}
			ssize_t increase_weak() {mutex.lock(); ssize_t ret = ++weak_count; mutex.unlock(); return ret;}
			ssize_t decrease_weak() {mutex.lock(); ssize_t ret = --weak_count; mutex.unlock(); return ret;}

			volatile ssize_t use_count;
			volatile ssize_t weak_count;
			sync::Mutex mutex;
		};

		template<LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class counted_base: public choose_lock_policy<LockPol>
		{
		public:
			virtual ~counted_base() noexcept = default;

			counted_base() noexcept
				: choose_lock_policy<LockPol>(1, 1)
			{
			}

			void release() noexcept
			{
				if (!choose_lock_policy<LockPol>::sub_use_ref_count())
				{
					dispose();
					if (!choose_lock_policy<LockPol>::sub_weak_ref_count())
						destroy();
				}
			}

			void weak_release() noexcept
			{
				if (!choose_lock_policy<LockPol>::sub_weak_ref_count())
					destroy();
			}

			virtual void* get_deleter(const std::type_info&) noexcept = 0;

		private:
			virtual void destroy() noexcept
			{
				delete this;
			}

			virtual void dispose() noexcept = 0;

			counted_base(const counted_base&) = delete;
			counted_base& operator =(const counted_base&) = delete;
		};

	}
}

#endif
