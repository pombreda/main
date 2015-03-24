#ifndef BASIS_MEMORY_FWD_HPP_
#define BASIS_MEMORY_FWD_HPP_

namespace simstd {

	template<typename Type>
	class allocator;

	template<typename Type, typename HeapType>
	class AllocatorHeap;

	template<typename Allocator>
	struct allocator_traits;

	template<typename Type>
	struct default_delete;

	template<typename Type, typename Deleter = default_delete<Type>>
	class unique_ptr;

	template<typename Type, typename... Args>
	unique_ptr<Type> make_unique(Args&&... args);

	template<typename Type>
	class shared_ptr;

	template<typename Type>
	class weak_ptr;

	template<typename Type>
	struct owner_less;

	template<typename Type>
	class enable_shared_from_this;

	namespace pvt {
		enum class LockPolicy: ssize_t {
			ATOMIC,
			MUTEX,
			NONE,
		};

		const LockPolicy DEFAULT_LOCK_POLICY = LockPolicy::ATOMIC;

		struct make_shared_tag {};

		template<LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class shared_count;

		template<LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class weak_count;

		template<typename Type, LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class shared_ptr_base;

		template<typename Type, LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class weak_ptr_base;

		template<typename Type, LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class enable_shared_from_this;
	}

}

#endif
