#ifndef EXCIS_CONNECTION_REMOTEI_HPP_
#define EXCIS_CONNECTION_REMOTEI_HPP_

namespace connection
{
	struct RemoteI
	{
		virtual ~RemoteI() = default;

		ustring get_host() const;

		void connect();
		void connect(const ustring& host, const wchar_t* user = nullptr, const wchar_t* pass = nullptr);

		void disconnect() {return do_disconnect();}

	private:
		virtual ustring do_get_host() const = 0;
		virtual void do_connect(const ustring& host, const wchar_t* user, const wchar_t* pass) = 0;
		virtual void do_disconnect() = 0;
	};

	using Remote = simstd::unique_ptr<RemoteI>;

	Remote create();
	Remote create(const ustring& host, const wchar_t* user = nullptr, const wchar_t* pass = nullptr);
}

#endif
