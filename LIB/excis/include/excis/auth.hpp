#ifndef EXCIS_AUTH_HPP_
#define EXCIS_AUTH_HPP_

#include <basis/configure.hpp>
#include <basis/simstd/iosfwd>

namespace auth
{
	class Credential
	{
		struct native_impl_type;

	public:
		using native_handle_type = native_impl_type*;

		~Credential();

		Credential(const wchar_t* name);
		Credential(Credential&& other);

		Credential& operator =(Credential&& other);

		void swap(Credential& other);

		ustring marshal() const;
		ustring comment() const;
		ustring name() const;
		ustring alias() const;
		ustring user() const;
		ustring pass() const;
		size_t pass_size() const;
		size_t flags() const;
		size_t type() const;
		size_t persist() const;

		static void add(const wchar_t* name, const wchar_t* pass, const wchar_t* target = nullptr);
		static void del(const wchar_t* name);

	private:
		native_handle_type cred;
		bool               owned;

		Credential(native_handle_type handle);

		friend struct Credentials;
	};

	struct Credentials
	{
		struct native_impl_type;

	public:
		using native_handle_type = native_impl_type*;

		~Credentials();

		Credentials();

		void update();

		bool empty() const;
		size_t size() const;

		Credential at(size_t ind) const;

	private:
		native_handle_type m_creds;
		size_t             m_size;
	};

	void parse_username(const wchar_t* fullname, ustring& dom, ustring& name);
}

#endif
