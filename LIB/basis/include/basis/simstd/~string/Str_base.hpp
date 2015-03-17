#ifndef BASIS_SIMSTD_STRING_STR_BASE_HPP_
#define BASIS_SIMSTD_STRING_STR_BASE_HPP_

namespace simstd
{
	namespace pvt
	{
		template<typename Type, typename Allocator>
		struct StrImpl: public Allocator
		{
			StrImpl(size_t capa, const Allocator& allocator) :
				Allocator(allocator),
				m_size(0),
				m_capa(capa),
				m_refc(1)
			{
				*m_data = m_terminal_char;
			}

			void increase_ref() noexcept {++m_refc;}
			bool decrease_ref() noexcept {return m_refc-- == 1;}
			bool is_shared() noexcept {return m_refc > 1;}
			size_t get_size() const noexcept {return m_size;}
			size_t get_capa() const noexcept {return m_capa;}
			void set_size(size_t size) noexcept {m_size = size; m_data[size] = m_terminal_char;}

			const Allocator& get_allocator() const noexcept {return *static_cast<Allocator*>(const_cast<StrImpl*>(this));}

			Type* get_data() const noexcept {return const_cast<Type*>(m_data);}
			Type* get_end() const noexcept {return const_cast<Type*>(m_data) + m_size;}

		private:
			size_t m_size;
			size_t m_capa;
			size_t m_refc;
			Type m_data[1];

			static const Type m_terminal_char = static_cast<Type>(0);
		};

		template<typename Type, typename Allocator>
		class StrBase
		{
		public:
			~StrBase();

			StrBase(const Allocator& allocator, size_t capa = 0);
			StrBase(const StrBase& other);
			StrBase(const StrBase& other, const Allocator& allocator);
			StrBase(StrBase&& other);
			StrBase(StrBase&& other, const Allocator& allocator);

			const Allocator& get_allocator() const;

		protected:
			typedef StrImpl<Type, Allocator> impl_type;
			typedef typename Allocator::template rebind<impl_type>::other ImpAllocator;
			typedef typename Allocator::template rebind<char>::other      RawAllocator;

			impl_type* m_impl;

		private:
			static impl_type* new_impl(size_t capa, const Allocator& allocator);
			static void       del_impl(const impl_type* ptr);

			const size_t MIN_CAPACITY = 4;
		};

		template<typename T, typename A>
		StrBase<T, A>::~StrBase()
		{
			if (m_impl && m_impl->decrease_ref())
				del_impl(m_impl);
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(const A& allocator, size_t capa) :
			m_impl()
		{
			m_impl = new_impl(simstd::max(MIN_CAPACITY, capa), allocator);
			CRT_ASSERT(m_impl);
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(const StrBase& other) :
			m_impl(other.m_impl)
		{
			CRT_ASSERT(m_impl);
			m_impl->increase_ref();
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(const StrBase& other, const A& allocator) :
			m_impl(other.m_impl)
		{
			CRT_ASSERT(m_impl);
			if (allocator == other.get_allocator()) {
				m_impl->increase_ref();
			} else {
				m_impl = new_impl(other.m_impl->get_capa(), allocator);
			}
			CRT_ASSERT(m_impl);
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(StrBase&& other) :
			m_impl()
		{
			using simstd::swap;
			swap(m_impl, other.m_impl);
			CRT_ASSERT(m_impl);
		}

		template<typename T, typename A>
		StrBase<T, A>::StrBase(StrBase&& other, const A& allocator) :
			m_impl()
		{
			if (allocator == other.get_allocator()) {
				using simstd::swap;
				swap(m_impl, other.m_impl);
			} else {
				m_impl = new_impl(other.m_impl->get_capa(), allocator);
			}
			CRT_ASSERT(m_impl);
		}

		template<typename T, typename A>
		const A& StrBase<T, A>::get_allocator() const
		{
			CRT_ASSERT(m_impl);
			return m_impl->get_allocator();
		}

		template<typename T, typename A>
		typename StrBase<T, A>::impl_type* StrBase<T, A>::new_impl(size_t capa, const A& allocator)
		{
			RawAllocator rawAlloc(allocator);
			size_t size = capa * sizeof(T) + sizeof(impl_type);
			auto ret = reinterpret_cast<StrBase<T, A>::impl_type*>(rawAlloc.allocate(size));
			CRT_ASSERT(ret);

			ImpAllocator implAlloc(allocator);
			implAlloc.construct(ret, capa, allocator);
			return ret;
		}

		template<typename T, typename A>
		void StrBase<T, A>::del_impl(const impl_type* ptr)
		{
			auto impl = const_cast<impl_type*>(ptr);
			ImpAllocator allocator = impl->get_allocator();
			allocator.destroy(impl);
			allocator.deallocate(impl, 1);
		}
	}
}

#endif
