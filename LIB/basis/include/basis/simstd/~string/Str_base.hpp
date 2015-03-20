#ifndef BASIS_SIMSTD_STRING_STR_BASE_HPP_
#define BASIS_SIMSTD_STRING_STR_BASE_HPP_

namespace simstd
{
	namespace pvt
	{
		template<typename Type, typename Allocator>
		struct basic_string_base_impl: public Allocator
		{
			basic_string_base_impl(size_t capa, const Allocator& allocator) :
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

			const Allocator& get_allocator() const noexcept {return *static_cast<const Allocator*>(this);}

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
		class basic_string_base
		{
		public:
			~basic_string_base();

			basic_string_base(const Allocator& allocator, size_t capa = 0);
			basic_string_base(const basic_string_base& other);
			basic_string_base(const basic_string_base& other, const Allocator& allocator);
			basic_string_base(basic_string_base&& other);
			basic_string_base(basic_string_base&& other, const Allocator& allocator);

		protected:
			using impl_type = basic_string_base_impl<Type, Allocator>;
			impl_type* m_impl;

		private:
			using impl_allocator = typename Allocator::template rebind<impl_type>::other;
			using raw_allocator = typename Allocator::template rebind<char>::other;

			static impl_type* new_impl(size_t capa, const Allocator& allocator);
			static void       del_impl(const impl_type* ptr);

			const size_t MIN_CAPACITY = 4;
		};

		template<typename T, typename A>
		basic_string_base<T, A>::~basic_string_base()
		{
			if (m_impl && m_impl->decrease_ref())
				del_impl(m_impl);
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(const A& allocator, size_t capa) :
			m_impl()
		{
			m_impl = new_impl(simstd::max(MIN_CAPACITY, capa), allocator);
			CRT_ASSERT(m_impl);
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(const basic_string_base& other) :
			m_impl(other.m_impl)
		{
			CRT_ASSERT(m_impl);
			m_impl->increase_ref();
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(const basic_string_base& other, const A& allocator) :
			m_impl(other.m_impl)
		{
			CRT_ASSERT(m_impl);
			if (allocator == other.m_impl->get_allocator()) {
				m_impl->increase_ref();
			} else {
				m_impl = new_impl(other.m_impl->get_capa(), allocator);
			}
			CRT_ASSERT(m_impl);
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(basic_string_base&& other) :
			m_impl(other.m_impl)
		{
			CRT_ASSERT(m_impl);
			m_impl->increase_ref();
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(basic_string_base&& other, const A& allocator) :
			m_impl(other.m_impl)
		{
			CRT_ASSERT(m_impl);
			if (allocator == other.m_impl->get_allocator()) {
				m_impl->increase_ref();
			} else {
				m_impl = new_impl(other.m_impl->get_capa(), allocator);
			}
			CRT_ASSERT(m_impl);
		}

		template<typename T, typename A>
		typename basic_string_base<T, A>::impl_type* basic_string_base<T, A>::new_impl(size_t capa, const A& allocator)
		{
			raw_allocator rawAlloc(allocator);
			size_t size = capa * sizeof(T) + sizeof(impl_type);
			auto ret = reinterpret_cast<basic_string_base<T, A>::impl_type*>(rawAlloc.allocate(size));
			CRT_ASSERT(ret);

			impl_allocator implAlloc(allocator);
			implAlloc.construct(ret, capa, allocator);
			return ret;
		}

		template<typename T, typename A>
		void basic_string_base<T, A>::del_impl(const impl_type* ptr)
		{
			auto impl = const_cast<impl_type*>(ptr);
			impl_allocator allocator = impl->get_allocator();
			allocator.destroy(impl);
			allocator.deallocate(impl, 1);
		}
	}
}

#endif
