#ifndef BASIS_MUTEX_UNIQUE_LOCK_HPP_
#define BASIS_MUTEX_UNIQUE_LOCK_HPP_

namespace simstd
{
	template<typename Mutex>
	class unique_lock
	{
		using this_type = unique_lock<Mutex>;

	public:
		typedef Mutex mutex_type;

		~unique_lock()
		{
			if (m_owns)
				unlock();
		}

		unique_lock() noexcept :
			m_sync(nullptr),
			m_owns(false)
		{
		}

		unique_lock(mutex_type& mutex) :
			m_sync(&mutex),
			m_owns(false)
		{
			lock();
		}

		unique_lock(mutex_type& mutex, defer_lock_t) noexcept :
			m_sync(&mutex),
			m_owns(false)
		{
		}

		unique_lock(mutex_type& mutex, try_to_lock_t) :
			m_sync(&mutex),
			m_owns(m_sync->try_lock())
		{
		}

		unique_lock(mutex_type& mutex, adopt_lock_t) :
			m_sync(&mutex),
			m_owns(true)
		{
		}

		unique_lock(this_type&& other) noexcept :
			m_sync(nullptr),
			m_owns(false)
		{
			swap(other);
		}

		this_type& operator =(this_type&& other)
		{
			this_type(simstd::move(other)).swap(*this);
			return *this;
		}

		void lock()
		{
			if (m_sync && !m_owns) {
				m_sync->lock();
				m_owns = true;
			}
		}

		bool try_lock()
		{
			if (m_sync && !m_owns) {
				m_owns = m_sync->try_lock();
			}
			return m_owns;
		}

		void unlock()
		{
			if (m_sync && m_owns) {
				m_owns = false;
				m_sync->unlock();
			}
		}

		void swap(this_type& other) noexcept
		{
			using simstd::swap;
			swap(m_sync, other.m_sync);
			swap(m_owns, other.m_owns);
		}

		mutex_type* release() noexcept
		{
			mutex_type* ret = m_sync;
			m_sync = nullptr;
			m_owns = false;
			return ret;
		}

		bool owns_lock() const
		{
			return m_owns;
		}

		explicit operator bool() const
		{
			return owns_lock();
		}

		mutex_type* mutex() const
		{
			return m_sync;
		}

	private:
		unique_lock(const this_type&) = delete;
		unique_lock& operator =(const this_type&) = delete;

		mutex_type* m_sync;
		bool        m_owns;
	};

	template<typename Mutex>
	void swap(unique_lock<Mutex>& a, unique_lock<Mutex>& b) noexcept
	{
		a.swap(b);
	}
}

#endif
