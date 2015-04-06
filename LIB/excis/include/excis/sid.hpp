#ifndef WIN_NET_SID_HPP
#define WIN_NET_SID_HPP

#include <basis/configure.hpp>
#include <basis/simstd/iosfwd>

#ifndef PSIDFromPACE
#define PSIDFromPACE(pACE) ((PSID)(&((pACE)->SidStart)))
#endif

namespace Ext
{
	/// Security Identifier (Идентификатор безопасности) -
	/// структура данных переменной длины, которая идентифицирует учетную запись пользователя, группы,
	/// домена или компьютера
	struct Sid
	{
		using this_type = Sid;
		using value_type = PSID;
		using size_type = size_t;

		static bool is_valid(value_type in) noexcept;
		static void check(value_type in);
		static size_type size(value_type in);
		static size_type sub_authority_count(value_type in);
		static size_type get_rid(value_type in);

		// PSID to sid string
		static ustring as_str(value_type in);

		// PSID to name
		static void get_name_dom(value_type sid, ustring& name, ustring& dom, const wchar_t* srv = nullptr);
		static ustring get_name(value_type sid, const wchar_t* srv = nullptr);
		static ustring get_full_name(value_type sid, const wchar_t* srv = nullptr);
		static ustring get_domain(value_type sid, const wchar_t* srv = nullptr);

		static PSID clone(value_type sid);
		static PSID get_sid(WELL_KNOWN_SID_TYPE wns);
		static PSID get_sid(const wchar_t* name, const wchar_t* srv = nullptr);

		~Sid();

		explicit Sid(WELL_KNOWN_SID_TYPE wns): m_sid(this_type::get_sid(wns)) {}
		explicit Sid(value_type other): m_sid(this_type::clone(other)) {}
		explicit Sid(const wchar_t* name, const wchar_t* srv = nullptr): m_sid(this_type::get_sid(name, srv)) {}

		Sid(const this_type& rhs): m_sid(this_type::clone(rhs.m_sid)) {}
		Sid(this_type&& rhs);

		this_type& operator =(const this_type& rhs);
		this_type& operator =(this_type&& rhs);
		this_type& operator =(value_type rhs);

		bool operator ==(value_type rhs) const;
		bool operator ==(const this_type& rhs) const {return operator ==(rhs.m_sid);}

		bool operator !=(value_type rhs) const {return !operator ==(rhs);}
		bool operator !=(const this_type& rhs) const {return !operator ==(rhs.m_sid);}

		size_type size() const noexcept(false) {return this_type::size(m_sid);};

		bool is_valid() const noexcept {return this_type::is_valid(m_sid);}

		ustring as_str() const;

		ustring get_name() const;
		ustring get_full_name() const;
		ustring get_domain() const;

		void copy_to(value_type out, size_t size) const;

		PSID clone() const
		{
			return clone(m_sid);
		}

		operator value_type() const
		{
			return m_sid;
		}

		void detach(value_type& sid);

		void swap(this_type& rhs);

	protected:
		Sid(): m_sid() {}

		value_type m_sid;
	};

	struct SidString: public Sid {
		explicit SidString(const wchar_t* str)
		{
			init(str);
		}

		explicit SidString(const ustring& str);

	private:
		void init(const wchar_t* str);
	};

	bool is_admin();

	ustring get_token_user(HANDLE hToken);
}

#endif
