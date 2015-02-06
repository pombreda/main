#ifndef BASIS_MEMORY_SHARED_PTR_SHARED_PTR_HPP_
#define BASIS_MEMORY_SHARED_PTR_SHARED_PTR_HPP_

#include "~shared_ptr/forward.hpp"

namespace simstd1 {

	namespace pvt {

		template<typename Type, LockPolicy LockPol>
		class shared_ptr {
		public:
			using element_type = Type;

			constexpr shared_ptr() noexcept: _M_ptr(nullptr)
			{}

			template<typename OType>
			explicit shared_ptr(OType* ptr)
				: _M_ptr(ptr)
				, _M_refcount(ptr)
			{
				__glibcxx_function_requires(_ConvertibleConcept<_Tp1*, _Tp*>)
				static_assert(!std::is_void<OType>::value, "incomplete type" );
				static_assert(sizeof(OType) > 0, "incomplete type" );
				__enable_shared_from_this_helper(_M_refcount, ptr, ptr);
			}

			template<typename OType, typename Deleter>
			shared_ptr(OType* ptr, Deleter deleter)
				: _M_ptr(ptr)
				, _M_refcount(ptr, deleter)
			{
				__glibcxx_function_requires(_ConvertibleConcept<_Tp1*, _Tp*>)
				__enable_shared_from_this_helper(_M_refcount, ptr, ptr);
			}

			template<typename OType, typename Deleter, typename Allocator>
			shared_ptr(OType* ptr, Deleter deleter, Allocator allocator)
				: _M_ptr(ptr)
				, _M_refcount(ptr, deleter, simstd::move(allocator))
			{
				__glibcxx_function_requires(_ConvertibleConcept<_Tp1*, _Tp*>)
				__enable_shared_from_this_helper(_M_refcount, ptr, ptr);
			}

			template<typename Deleter>
			shared_ptr(nullptr_t ptr, Deleter deleter)
				: _M_ptr(0)
				, _M_refcount(ptr, deleter)
			{
			}

			template<typename Deleter, typename Allocator>
			shared_ptr(nullptr_t ptr, Deleter deleter, Allocator allocator)
				: _M_ptr(0)
				, _M_refcount(ptr, deleter, simstd::move(allocator))
			{
			}

			template<typename OType>
			shared_ptr(const shared_ptr<OType, LockPol>& other, Type* ptr) noexcept
				: _M_ptr(ptr)
				, _M_refcount(other._M_refcount)
			{
			}

			template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
			shared_ptr(const shared_ptr<OType, LockPol>& other) noexcept
				: _M_ptr(other._M_ptr)
				, _M_refcount(other._M_refcount)
			{
			}

			shared_ptr(shared_ptr&& other) noexcept
				: _M_ptr(other._M_ptr)
			{
				_M_refcount.swap(other._M_refcount);
				other._M_ptr = nullptr;
			}

			template<typename OType, typename = typename std::enable_if<std::is_convertible<OType*, Type*>::value>::type>
			shared_ptr(shared_ptr<OType, LockPol>&& other) noexcept
				: _M_ptr(other._M_ptr)
			{
				_M_refcount.swap(other._M_refcount);
				other._M_ptr = nullptr;
			}

			template<typename OType>
			explicit shared_ptr(const weak_ptr<OType, LockPol>& other)
				: _M_ptr(other._M_ptr)
				, _M_refcount(other._M_refcount)
			{
			}

			template<typename OType, typename Deleter>
			shared_ptr(simstd::unique_ptr<OType, Deleter>&& other)
				: _M_ptr(other.get())
			  {
				__glibcxx_function_requires(_ConvertibleConcept<_Tp1*, _Tp*>)
				auto __raw = _S_raw_ptr(__r.get());
				_M_refcount = __shared_count<_Lp>(std::move(__r));
				__enable_shared_from_this_helper(_M_refcount, __raw, __raw);
			  }

		/* TODO: use delegating constructor */
		constexpr __shared_ptr(nullptr_t) noexcept
		: _M_ptr(0), _M_refcount()
		{}

		template<typename _Tp1>
		__shared_ptr&
		operator=(const __shared_ptr<_Tp1, _Lp>& __r) noexcept
		{
			_M_ptr = __r._M_ptr;
			_M_refcount = __r._M_refcount; // __shared_count::op= doesn't throw
			return *this;
		}

#if _GLIBCXX_USE_DEPRECATED
		template<typename _Tp1>
		__shared_ptr&
		operator=(std::auto_ptr<_Tp1>&& __r)
		{
			__shared_ptr(std::move(__r)).swap(*this);
			return *this;
		}
#endif

		__shared_ptr&
		operator=(__shared_ptr&& __r) noexcept
		{
			__shared_ptr(std::move(__r)).swap(*this);
			return *this;
		}

		template<class _Tp1>
		__shared_ptr&
		operator=(__shared_ptr<_Tp1, _Lp>&& __r) noexcept
		{
			__shared_ptr(std::move(__r)).swap(*this);
			return *this;
		}

		template<typename _Tp1, typename _Del>
		__shared_ptr&
		operator=(std::unique_ptr<_Tp1, _Del>&& __r)
		{
			__shared_ptr(std::move(__r)).swap(*this);
			return *this;
		}

		void
		reset() noexcept
		{	__shared_ptr().swap(*this);}

		template<typename _Tp1>
		void
		reset(_Tp1* __p) // _Tp1 must be complete.
		{
			// Catch self-reset errors.
			_GLIBCXX_DEBUG_ASSERT(__p == 0 || __p != _M_ptr);
			__shared_ptr(__p).swap(*this);
		}

		template<typename _Tp1, typename _Deleter>
		void
		reset(_Tp1* __p, _Deleter __d)
		{	__shared_ptr(__p, __d).swap(*this);}

		template<typename _Tp1, typename _Deleter, typename _Alloc>
		void
		reset(_Tp1* __p, _Deleter __d, _Alloc __a)
		{	__shared_ptr(__p, __d, std::move(__a)).swap(*this);}

		// Allow class instantiation when _Tp is [cv-qual] void.
		typename std::add_lvalue_reference<_Tp>::type
		operator*() const noexcept
		{
			_GLIBCXX_DEBUG_ASSERT(_M_ptr != 0);
			return *_M_ptr;
		}

		_Tp*
		operator->() const noexcept
		{
			_GLIBCXX_DEBUG_ASSERT(_M_ptr != 0);
			return _M_ptr;
		}

		_Tp*
		get() const noexcept
		{	return _M_ptr;}

		explicit operator bool() const // never throws
		{	return _M_ptr == 0 ? false : true;}

		bool
		unique() const noexcept
		{	return _M_refcount._M_unique();}

		long
		use_count() const noexcept
		{	return _M_refcount._M_get_use_count();}

		void
		swap(__shared_ptr<_Tp, _Lp>& __other) noexcept
		{
			std::swap(_M_ptr, __other._M_ptr);
			_M_refcount._M_swap(__other._M_refcount);
		}

		template<typename _Tp1>
		bool
		owner_before(__shared_ptr<_Tp1, _Lp> const& __rhs) const
		{	return _M_refcount._M_less(__rhs._M_refcount);}

		template<typename _Tp1>
		bool
		owner_before(__weak_ptr<_Tp1, _Lp> const& __rhs) const
		{	return _M_refcount._M_less(__rhs._M_refcount);}

#ifdef __GXX_RTTI
	protected:
		// This constructor is non-standard, it is used by allocate_shared.
		template<typename _Alloc, typename... _Args>
		__shared_ptr(_Sp_make_shared_tag __tag, const _Alloc& __a,
			_Args&&... __args)
		: _M_ptr(), _M_refcount(__tag, (_Tp*)0, __a,
			std::forward<_Args>(__args)...)
		{
			// _M_ptr needs to point to the newly constructed object.
			// This relies on _Sp_counted_ptr_inplace::_M_get_deleter.
			void* __p = _M_refcount._M_get_deleter(typeid(__tag));
			_M_ptr = static_cast<_Tp*>(__p);
			__enable_shared_from_this_helper(_M_refcount, _M_ptr, _M_ptr);
		}
#else
		template<typename _Alloc>
		struct _Deleter
		{
			void operator()(_Tp* __ptr)
			{
				typedef allocator_traits<_Alloc> _Alloc_traits;
				_Alloc_traits::destroy(_M_alloc, __ptr);
				_Alloc_traits::deallocate(_M_alloc, __ptr, 1);
			}
			_Alloc _M_alloc;
		};

		template<typename _Alloc, typename... _Args>
		__shared_ptr(_Sp_make_shared_tag __tag, const _Alloc& __a,
			_Args&&... __args)
		: _M_ptr(), _M_refcount()
		{
			typedef typename _Alloc::template rebind<_Tp>::other _Alloc2;
			_Deleter<_Alloc2> __del = {_Alloc2(__a)};
			typedef allocator_traits<_Alloc2> __traits;
			_M_ptr = __traits::allocate(__del._M_alloc, 1);
			__try
			{
				// _GLIBCXX_RESOLVE_LIB_DEFECTS
				// 2070. allocate_shared should use allocator_traits<A>::construct
				__traits::construct(__del._M_alloc, _M_ptr,
					std::forward<_Args>(__args)...);
			}
			__catch(...)
			{
				__traits::deallocate(__del._M_alloc, _M_ptr, 1);
				__throw_exception_again;
			}
			__shared_count<_Lp> __count(_M_ptr, __del, __del._M_alloc);
			_M_refcount._M_swap(__count);
			__enable_shared_from_this_helper(_M_refcount, _M_ptr, _M_ptr);
		}
#endif

		template<typename _Tp1, _Lock_policy _Lp1, typename _Alloc,
		typename... _Args>
		friend __shared_ptr<_Tp1, _Lp1>
		__allocate_shared(const _Alloc& __a, _Args&&... __args);

		// This constructor is used by __weak_ptr::lock() and
		// shared_ptr::shared_ptr(const weak_ptr&, std::nothrow_t).
		__shared_ptr(const __weak_ptr<_Tp, _Lp>& __r, std::nothrow_t)
		: _M_refcount(__r._M_refcount, std::nothrow)
		{
			_M_ptr = _M_refcount._M_get_use_count() ? __r._M_ptr : nullptr;
		}

		friend class __weak_ptr<_Tp, _Lp>;

	private:
		void*
		_M_get_deleter(const std::type_info& __ti) const noexcept
		{	return _M_refcount._M_get_deleter(__ti);}

		template<typename _Tp1>
		static _Tp1*
		_S_raw_ptr(_Tp1* __ptr)
		{	return __ptr;}

		template<typename _Tp1>
		static auto
		_S_raw_ptr(_Tp1 __ptr) -> decltype(std::__addressof(*__ptr))
		{	return std::__addressof(*__ptr);}

		template<typename _Tp1, _Lock_policy _Lp1> friend class __shared_ptr;
		template<typename _Tp1, _Lock_policy _Lp1> friend class __weak_ptr;

		template<typename _Del, typename _Tp1, _Lock_policy _Lp1>
		friend _Del* get_deleter(const __shared_ptr<_Tp1, _Lp1>&) noexcept;

		element_type* _M_ptr;
		shared_count<LockPol> _M_refcount;
	};

}

}

#endif
