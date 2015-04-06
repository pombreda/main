#ifndef EXCIS_SERVICE_ENUM_HPP_
#define EXCIS_SERVICE_ENUM_HPP_

namespace service
{
	struct Enum:
		public sync::Observable,
		private simstd::vector<Info>
	{
		using base_type = simstd::vector<Info>;
		using value_type = base_type::value_type;
		using size_type = base_type::size_type;
		using iterator = base_type::iterator;
		using const_iterator = base_type::const_iterator;

		using base_type::begin;
		using base_type::cbegin;
		using base_type::end;
		using base_type::cend;
		using base_type::size;
		using base_type::empty;
		using base_type::operator[];

		~Enum();
		Enum(EnumerateType type, const ustring& host = ustring(), const wchar_t* user = nullptr, const wchar_t* pass = nullptr);

		void update();

		EnumerateType get_type() const;
		void set_type(EnumerateType type);

		ustring get_host() const;
		void set_host(const ustring& host = ustring(), const wchar_t* user = nullptr, const wchar_t* pass = nullptr);

		iterator find(const ustring& name);
		const_iterator find(const ustring& name) const;

		void add(const CreateRequest& request);

		void del(iterator it);

		void start(iterator it);
		void stop(iterator it);
		void restart(iterator it);
		void contin(iterator it);
		void pause(iterator it);
		void set_config(iterator it, const ConfigRequest& request);

		void start_batch();
		void notify_changed();
		void stop_batch();

		void set_wait_state(bool new_state);
		bool get_wait_state() const;

		void set_wait_timeout(size_t timeout_msec);
		size_t get_wait_timeout() const;

	private:
		simstd::unique_ptr<Filter> m_filter;
		EnumerateType              m_type;
		size_t                     m_wait_timout;
		size_t                     m_wait_state :1;
		size_t                     m_batch_started :1;
	};
}

#endif
