#ifndef _LIBCOM_STD_HPP_
#define _LIBCOM_STD_HPP_

#include <libbase/std.hpp>
#include <liblog/logger.hpp>
#include <algorithm>

namespace Com {

	void init();

	HRESULT ConvertErrorToHRESULT(LONG error);

	HRESULT ConvertBoolToHRESULT(bool result);

	///====================================================================================== Object
	template<typename Interface>
	class Object {
		typedef Object this_type;
		typedef Interface * pointer;
		typedef const Interface * const_pointer;

	public:
		~Object()
		{
			Release();
		}

		Object() :
			m_obj(nullptr)
		{
		}

		explicit Object(const_pointer param) :
			m_obj((pointer)param)
		{ // caller must not Release param
		}

		explicit Object(const VARIANT & param) :
			m_obj((pointer)param.ppunkVal)
		{
			m_obj->AddRef();
		}

		Object(const this_type & right) :
			m_obj(right.m_obj)
		{
			if (m_obj) {
				m_obj->AddRef();
			}
		}

		template<typename OtherInterface>
		Object(const Object<OtherInterface> & right) :
			m_obj(static_cast<OtherInterface*>(right))
		{
			if (m_obj) {
				m_obj->AddRef();
			}
		}

		this_type & operator =(const_pointer rhs)
		{ // caller must not Release rhs
			if (m_obj != rhs) {
				this_type tmp(rhs);
				swap(tmp);
			}
			return *this;
		}
		this_type & operator =(const this_type & rhs)
		{
			if (m_obj != rhs.m_obj) {
				this_type tmp(rhs);
				swap(tmp);
			}
			return *this;
		}

		template<typename OtherInterface>
		this_type & operator =(const Object<OtherInterface> & right)
		{
			if (m_obj != static_cast<OtherInterface*>(right)) {
				this_type tmp(right);
				swap(tmp);
			}
			return *this;
		}

		void Release()
		{
			if (m_obj) {
				m_obj->Release();
				m_obj = nullptr;
			}
		}

		operator bool() const
		{
			return m_obj;
		}
		operator pointer() const
		{
//			LogNoise(L"%p\n", m_obj);
			return m_obj;
		}

		pointer * operator &()
		{
			Release();
			return &m_obj;
		}
		pointer operator ->() const
		{
//			LogNoise(L"%p\n", m_obj);
			return m_obj;
		}

		bool operator ==(const pointer rhs) const
		{
			return m_obj == rhs;
		}
		bool operator ==(const this_type & rhs) const
		{
			return m_obj == rhs.m_obj;
		}
		bool operator !=(const pointer rhs) const
		{
			return m_obj != rhs;
		}
		bool operator !=(const this_type & rhs) const
		{
			return m_obj != rhs.m_obj;
		}

		void attach(pointer & param)
		{
			Release();
			m_obj = param;
			param = nullptr;
		}
		void detach(pointer & param)
		{
			param = m_obj;
			m_obj = nullptr;
		}

		void swap(this_type & rhs)
		{
			using std::swap;
			swap(m_obj, rhs.m_obj);
		}

	private:
		pointer m_obj;
	};

}

#endif
