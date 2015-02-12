#ifndef BASIS_MEMORY_SHARED_PTR_FORWARD_HPP_
#define BASIS_MEMORY_SHARED_PTR_FORWARD_HPP_

namespace simstd1 {

	namespace pvt {

		enum class LockPolicy: ssize_t {
			ATOMIC,
			NONE,
			MUTEX,
		};

		const LockPolicy DEFAULT_LOCK_POLICY = LockPolicy::ATOMIC;

		struct make_shared_tag {};

		template<LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class shared_count;

		template<LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class weak_count;

		template<typename Type, LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class shared_ptr;

		template<typename Type, LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class weak_ptr;

		template<typename Type, LockPolicy LockPol = DEFAULT_LOCK_POLICY>
		class enable_shared_from_this;
	}

	// Forward declarations.
	template<typename Type>
	class shared_ptr;

	template<typename Type>
	class weak_ptr;

	template<typename Type>
	struct owner_less;

	template<typename Type>
	class enable_shared_from_this;

}

#endif
