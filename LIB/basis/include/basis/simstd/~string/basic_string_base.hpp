#ifndef BASIS_SIMSTD_STRING_STR_BASE_HPP_
#define BASIS_SIMSTD_STRING_STR_BASE_HPP_

namespace simstd
{
	namespace pvt
	{
		template<typename Type, typename Allocator>
		struct basic_string_base_impl: public Allocator
		{
			static basic_string_base_impl* create(const Allocator& allocator, size_t capa);

			basic_string_base_impl(const Allocator& allocator, size_t capa) :
				Allocator(allocator),
				capa(capa)
			{
				*data = static_cast<Type>(0);
			}

			void increase_ref() noexcept {++refc;}
			void decrease_ref() noexcept {if (refc-- == 1) dispose();}
			bool is_shared() const noexcept {return refc > 1;}
			size_t get_size() const noexcept {return size;}
			size_t get_capa() const noexcept {return capa;}
			void set_size(size_t size) noexcept {this->size = size; data[size] = static_cast<Type>(0);}

			Allocator& get_allocator() noexcept {return *static_cast<Allocator*>(this);}
			Type* get_data() noexcept {return data;}
			Type* get_end() noexcept {return data + size;}
			const Allocator& get_allocator() const noexcept {return *static_cast<const Allocator*>(this);}
			const Type* get_data() const noexcept {return data;}
			const Type* get_end() const noexcept {return data + size;}

		private:
			void dispose();

			size_t refc = 1;
			size_t size = 0;
			size_t capa;
			Type data[1];
		};

		template<typename T, typename A>
		basic_string_base_impl<T, A>* basic_string_base_impl<T, A>::create(const A& allocator, size_t capa)
		{
			const size_t MIN_CAPACITY = 4;

			using raw_allocator = typename A::template rebind<char>::other;
			raw_allocator rawAlloc(allocator);
			size_t size = sizeof(basic_string_base_impl<T, A>) + sizeof(T) * simstd::max(MIN_CAPACITY, capa);
			auto ret = reinterpret_cast<basic_string_base_impl<T, A>*>(rawAlloc.allocate(size));
			CRT_ASSERT(ret);

			using impl_allocator = typename A::template rebind<basic_string_base_impl<T, A>>::other;
			impl_allocator implAlloc(allocator);
			implAlloc.construct(ret, allocator, capa);
			return ret;
		}

		template<typename T, typename A>
		void basic_string_base_impl<T, A>::dispose()
		{
			using impl_allocator = typename A::template rebind<basic_string_base_impl<T, A>>::other;
			impl_allocator allocator = this->get_allocator();
			allocator.destroy(this);
			allocator.deallocate(this, 1);
		}

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
			impl_type* impl;
		};

		template<typename T, typename A>
		basic_string_base<T, A>::~basic_string_base()
		{
			impl->decrease_ref();
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(const A& allocator, size_t capa) :
			impl(impl_type::create(allocator, capa))
		{
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(const basic_string_base& other) :
			impl(other.impl)
		{
			impl->increase_ref();
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(const basic_string_base& other, const A& allocator) :
			impl(other.impl)
		{
			if (allocator == other.impl->get_allocator()) {
				impl->increase_ref();
			} else {
				impl = impl_type::create(allocator, other.impl->get_capa());
			}
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(basic_string_base&& other) :
			impl(other.impl)
		{
			impl->increase_ref();
		}

		template<typename T, typename A>
		basic_string_base<T, A>::basic_string_base(basic_string_base&& other, const A& allocator) :
			impl(other.impl)
		{
			if (allocator == other.impl->get_allocator()) {
				impl->increase_ref();
			} else {
				impl = impl_type::create(allocator, other.impl->get_capa());
			}
		}
	}
}

#endif
