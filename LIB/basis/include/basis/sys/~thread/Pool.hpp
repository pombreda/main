#ifndef BASIS_SYS_THREAD_POOL_HPP_
#define BASIS_SYS_THREAD_POOL_HPP_

#include <basis/simstd/vector>

namespace thread
{
	struct Pool:
		private pattern::Uncopyable,
		private simstd::vector<Unit>
	{
		typedef simstd::vector<Unit> base_type;
		using base_type::back;
		using base_type::begin;
		using base_type::end;
		using base_type::size;
		using base_type::empty;
		using base_type::operator[];

		Pool();

		~Pool();

		template<class... Args>
		void create_thread(Args&&... args)
		{
			base_type::emplace_back(thread::create(args...));
			m_handles.push_back(base_type::back()->get_handle());
		}

//		void push_back(Thread thread)
//		{
//			m_threads.push_back(thread);
//			m_handles.push_back(thread.get_handle());
//		}

//		Thread & operator [](size_t i)
//		{
//			return m_threads[i];
//		}

		sync::WaitResult_t wait_all(int64_t timeout = sync::TIMEOUT_INFINITE) const;

		sync::WaitResult_t wait_any(size_t& index, int64_t timeout = sync::TIMEOUT_INFINITE) const;

	private:
		simstd::vector<HANDLE> m_handles;
	};
}

#endif
