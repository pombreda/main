#ifndef BASIS_SYS_MEMORY_AUTO_BUF_HPP_
#define BASIS_SYS_MEMORY_AUTO_BUF_HPP_

namespace memory
{
	template<typename Type>
	struct auto_buf:
		private pattern::Uncopyable
	{
		using this_type = auto_buf;
		using value_type = Type;
		using size_type = size_t;

		~auto_buf()
		{
			must_be_pointer<Type>::constraints(ptr);
			HostFree(memory::heap::DefaultStat, ptr);
		}

		auto_buf() noexcept = default;

		explicit auto_buf(size_type size_bytes)
		{
			reserve(size_bytes);
		}

		auto_buf(this_type&& other)
		{
			swap(other);
		}

		this_type& operator =(this_type&& other)
		{
			if (ptr != other.ptr)
				this_type(simstd::move(other)).swap(*this);
			return *this;
		}

		void reserve(size_type new_size_bytes)
		{
			if (size() < new_size_bytes)
				ptr = static_cast<value_type>(HostRealloc(memory::heap::DefaultStat, ptr, new_size_bytes));
		}

		size_type size() const
		{
			return memory::size(ptr);
		}

		value_type operator &() const
		{
			return ptr;
		}

		value_type operator ->() const
		{
			CRT_ASSERT(ptr);
			return ptr;
		}

		operator value_type() const
		{
			return ptr;
		}

		value_type data() const
		{
			return ptr;
		}

		bool operator !() const
		{
			return ptr;
		}

		void attach(value_type& other)
		{
			HostFree(memory::heap::DefaultStat, ptr);
			ptr = other;
		}

		void detach(value_type& other)
		{
			other = ptr;
			ptr = nullptr;
		}

		void swap(value_type& other) noexcept
		{
			using simstd::swap;
			swap(ptr, other);
		}

		void swap(this_type& other) noexcept
		{
			using simstd::swap;
			swap(ptr, other.ptr);
		}

	private:
		value_type ptr = value_type();
	};

	template<typename Type>
	void swap(auto_buf<Type>& a, auto_buf<Type>& b)
	{
		a.swap(b);
	}
}

#endif
