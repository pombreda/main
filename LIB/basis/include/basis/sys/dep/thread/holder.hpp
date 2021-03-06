#ifndef LIBBASE_THREAD_HOLDER_HPP_
#define LIBBASE_THREAD_HOLDER_HPP_

#include <basis/configure.hpp>
#include <basis/ext/pattern.hpp>

#include <vector>

namespace Base {

	struct ThreadsHolder: private pattern::Uncopyable {

		~ThreadsHolder();

		void alert();

		void alert(void * data);

		void add(Thread && thread);

		void add(ThreadRoutine_i * routine);

		Thread & back();

		const Thread & back() const;

		Thread & operator [] (size_t i);

		const Thread & operator [] (size_t i) const ;

		bool wait_all(size_t timeout = WAIT_FOREVER) const;

		size_t wait_any(size_t timeout = WAIT_FOREVER) const;

		size_t size() const;

	private:
		std::vector<Thread::handle_t> m_handles;
		std::vector<Thread> m_threads;
	};

}

#endif
