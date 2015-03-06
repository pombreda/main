#ifndef BASIS_MUTEX_RECURSIVE_MUTEX_HPP_
#define BASIS_MUTEX_RECURSIVE_MUTEX_HPP_

namespace simstd {

	class recursive_mutex: private sync::Mutex
	{
		using base_type = sync::Mutex;

	public:
		using native_handle_type = base_type::native_handle_type;

		using base_type::lock;

		using base_type::try_lock;

		using base_type::unlock;

		using base_type::native_handle;
	};

}

#endif
