﻿#ifndef LIBBASE_REG_HPP_
#define LIBBASE_REG_HPP_

#include <basis/configure.hpp>
#include <basis/ext/pattern.hpp>

namespace Base {

	struct Register: private pattern::Uncopyable {
		~Register()
		{
			close();
		}

		Register() :
			m_key(nullptr)
		{
		}

		void close()
		{
			if (m_key) {
				::RegCloseKey(m_key);
				m_key = nullptr;
			}
		}

		bool open_key(ACCESS_MASK acc, const wchar_t* path, HKEY key = HKEY_CURRENT_USER )
		{
			close();
			bool ret = false;
			if (acc & KEY_WRITE)
				ret = ::RegCreateKeyExW(key, path, 0, nullptr, 0, acc, 0, &m_key, 0) == ERROR_SUCCESS;
			else
				ret = ::RegOpenKeyExW(key, path, 0, acc, &m_key) == ERROR_SUCCESS;
			return ret;
		}

		bool del(const wchar_t* name) const
		{
			return ::RegDeleteValueW(m_key, name) == ERROR_SUCCESS;
		}

		uint64_t get(const wchar_t* name, uint64_t def) const
		{
			if (m_key) {
				uint64_t value;
				DWORD size = sizeof(def);
				if (::RegQueryValueExW(m_key, name, nullptr, nullptr, (PBYTE)(&value), &size) == ERROR_SUCCESS)
					return value;
			}
			return def;
		}

		bool get(const wchar_t* name, wchar_t* value, DWORD size) const
		{
			if (m_key) {
				size *= sizeof(wchar_t);
				return ::RegQueryValueExW(m_key, name, nullptr, nullptr, (PBYTE)value, &size) == ERROR_SUCCESS;
			}
			return false;
		}

		bool set(const wchar_t* name, PCVOID value, size_t size, DWORD type = REG_BINARY) const
		{
			if (m_key) {
				return ::RegSetValueExW(m_key, name, 0, type, (PBYTE)value, size) == ERROR_SUCCESS;
			}
			return false;
		}

		void set(const wchar_t* name, uint64_t value) const
		{
			set(name, &value, sizeof(value), REG_QWORD);
		}

		void set(const wchar_t* name, uint32_t value) const
		{
			set(name, &value, sizeof(value), REG_DWORD);
		}

		void set(const wchar_t* name, const wchar_t* value) const
		{
			set(name, value, sizeof(wchar_t) * (wcslen(value) + 1), REG_SZ);
		}

	private:
		HKEY m_key;
	};

}

#endif
