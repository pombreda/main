﻿#include <libbase/mstring.hpp>

#include <libbase/memory.hpp>
#include <libbase/string.hpp>
#include <liblog/logger.hpp>

namespace Base {

	struct mstring::impl {
		~impl();

		explicit impl(PCWSTR in);

		impl(const impl & other);

		void push_back(PCWSTR str);

	private:
		PWSTR m_data;
		size_t m_capa;
		size_t m_size;

		friend class mstring;
	};

	mstring::impl::~impl()
	{
		Memory::free(m_data);
	}

	mstring::impl::impl(PCWSTR in) :
		m_data(nullptr),
		m_capa(0),
		m_size(0)
	{
		PCWSTR ptr = in;
		while (ptr && *ptr) {
			ptr += (Cstr::length(ptr) + 1);
			++m_size;
		}
		m_capa = ptr - in;
		if (m_capa) {
			m_data = Memory::calloc<wchar_t*>(m_capa);
			Memory::copy(m_data, in, m_capa * sizeof(wchar_t));
		}
	}

	mstring::impl::impl(const impl & other) :
		m_data(nullptr),
		m_capa(other.m_capa),
		m_size(other.m_size)
	{
		if (m_capa) {
			m_data = Memory::calloc<wchar_t*>(m_capa);
			Memory::copy(m_data, other.m_data, m_capa * sizeof(wchar_t));
		}
	}

	void mstring::impl::push_back(PCWSTR str)
	{
		if (!Cstr::is_empty(str)) {
			size_t size = Cstr::length(str) + 1;
			++m_size;
			size_t new_index = m_capa;
			m_capa += size;
			Memory::realloc(m_data, sizeof(wchar_t) * m_capa);
			Memory::copy(&m_data[new_index], str, size * sizeof(wchar_t));
		}
	}

	mstring::~mstring()
	{
		delete m_str;
	}

	mstring::mstring(PCWSTR in) :
		m_str(new impl(in))
	{
	}

	mstring::mstring(const mstring & other):
		m_str(new impl(*other.m_str))
	{
	}

	mstring & mstring::operator =(const mstring & other)
	{
		if (this != &other)
			mstring(other).swap(*this);
		return *this;
	}

	mstring::mstring(mstring && rhs):
		m_str(rhs.m_str)
	{
		rhs.m_str = nullptr;
	}

	mstring & mstring::operator =(mstring && rhs) {
		if (this != &rhs) {
			impl * tmp = m_str;
			m_str = rhs.m_str;
			rhs.m_str = nullptr;
			delete tmp;
		}
		return *this;
	}

	void mstring::push_back(PCWSTR str)
	{
		m_str->push_back(str);
	}

	size_t mstring::size() const
	{
		return m_str->m_size;
	}

	size_t mstring::capacity() const
	{
		return m_str->m_capa;
	}

	PCWSTR mstring::c_str() const
	{
		return m_str->m_data;
	}

	PCWSTR mstring::operator [](size_t index) const
	{
		PCWSTR ptr = c_str();
		size_t cnt = 0;
		while (*ptr && (cnt++ < index)) {
			ptr += (Cstr::length(ptr) + 1);
		}
		return ptr;
	}

	void mstring::swap(mstring & other)
	{
		using std::swap;
		swap(m_str, other.m_str);
	}

}
