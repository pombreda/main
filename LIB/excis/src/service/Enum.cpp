#include "service_pvt.hpp"

#include <excis/exception.hpp>

#include <basis/sys/logger.hpp>

#include <basis/simstd/algorithm>

namespace service
{
	class Filter
	{
	public:
		~Filter();
		Filter(const ustring& host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr, EnumerateType = EnumerateType::SERVICES);

		void swap(Filter& other);

		const connection::Remote& get_connection() const;
		const Manager& get_read_manager() const;
		const Manager& get_write_manager() const;

		ustring get_host() const;
		void set_host(const ustring& host = ustring(), PCWSTR user = nullptr, PCWSTR pass = nullptr);

	private:
		mutable bool writable;
		connection::Remote connection;
		mutable Manager scm;
	};

	Filter::~Filter()
	{
		LogTraceObjLn();
	}

	Filter::Filter(const ustring& host, PCWSTR user, PCWSTR pass, EnumerateType type) :
		writable(false),
		connection(connection::create(host, user, pass)),
		scm(connection, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE)
	{
		LogTraceObj(L"('%s', '%s', 0x%IX)\n", host.c_str(), user, (size_t)type);
	}

	void Filter::swap(Filter& other)
	{
		using simstd::swap;
		swap(writable, other.writable);
		swap(scm, other.scm);
		swap(connection, other.connection);
	}

	const connection::Remote& Filter::get_connection() const
	{
		return connection;
	}

	const Manager& Filter::get_read_manager() const
	{
		return scm;
	}

	const Manager& Filter::get_write_manager() const
	{
		if (!writable) {
			scm = Manager(connection, SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_CREATE_SERVICE);
			writable = true;
		}
		return scm;
	}

	ustring Filter::get_host() const
	{
		return connection->get_host();
	}

	void Filter::set_host(const ustring& host, PCWSTR user, PCWSTR pass)
	{
		LogTrace(L"host: '%s', user: '%s'\n", host.c_str(), user);
		Filter(host, user, pass).swap(*this);
	}

	///==================================================================================== Services
	Enum::~Enum()
	{
		LogTraceObjLn();
	}

	Enum::Enum(EnumerateType type, const ustring& host, PCWSTR user, PCWSTR pass) :
		m_filter(),
		m_type(type),
		m_wait_timout(10 * 1000),
		m_wait_state(false),
		m_batch_started(false)
	{
		LogTraceObj(L"begin (0x%IX, '%s', user: '%s')\n", (size_t)type, host.c_str(), user);
		m_filter = simstd::make_unique<Filter>(host, user, pass);
		LogTraceObj(L"end\n");
	}

	EnumerateType Enum::get_type() const
	{
		return m_type;
	}

	void Enum::set_type(EnumerateType type)
	{
		LogTrace(L"[0x%X] (0x%X)\n", (uint32_t)m_type, (uint32_t)type);
		if (type != m_type) {
			m_type = type;
			update();
			notify_changed();
		} else {
			LogAtten(L"type 0x%X is already set\n", (uint32_t)type);
		}
	}

	ustring Enum::get_host() const
	{
		return m_filter->get_host();
	}

	void Enum::set_host(const ustring& host, PCWSTR user, PCWSTR pass)
	{
		LogTrace(L"['%s'] ('%s', %s)\n", m_filter->get_host().c_str(), host.c_str(), user);
		m_filter->set_host(host, user, pass);
		update();
		notify_changed();
	}

	void Enum::update()
	{
		// filter is changed
		LogTrace(L"[type: 0x%X]\n", (uint32_t)get_type());
		DWORD bufNeed = 0, dwNumberOfService = 0;
		::EnumServicesStatusExW(m_filter->get_read_manager(), SC_ENUM_PROCESS_INFO, (DWORD)get_type(), SERVICE_STATE_ALL, nullptr, 0, &bufNeed, &dwNumberOfService, nullptr, nullptr);
		CheckApi(::GetLastError() == ERROR_MORE_DATA);

		memory::auto_buf<LPENUM_SERVICE_STATUS_PROCESSW> enum_svc(bufNeed);
		CheckApi(::EnumServicesStatusExW(m_filter->get_read_manager(), SC_ENUM_PROCESS_INFO, (DWORD)get_type(), SERVICE_STATE_ALL, (PBYTE)enum_svc.data(), enum_svc.size(), &bufNeed, &dwNumberOfService, nullptr, nullptr));

		clear();
		reserve(dwNumberOfService);
		for (ULONG i = 0; i < dwNumberOfService; ++i) {
			emplace_back(m_filter->get_read_manager(), enum_svc.data()[i]);
		}
		LogTrace(L"-> cached: %Iu\n", size());
	}

	Enum::iterator Enum::find(const ustring& name)
	{
		return simstd::find(begin(), end(), name);
	}

	Enum::const_iterator Enum::find(const ustring& name) const
	{
		return simstd::find(begin(), end(), name);
	}

	void Enum::add(const CreateRequest& request)
	{
		LogTrace(L"('%s')\n", request.get_name());
		emplace_back(request.get_name(), request.execute(m_filter->get_write_manager()));
		notify_changed();
	}

	void Enum::del(iterator it)
	{
		CRT_ASSERT(it != end());
		LogTrace(L"(%s)\n", it->name.c_str());
//		try {
		Item::del(m_filter->get_read_manager(), it->name.c_str());
		erase(it);
		notify_changed();
//		} catch (Abstract & e) {
//			Rethrow(e, msg);
//		}
	}

	void Enum::start(iterator it)
	{
		CRT_ASSERT(it != end());
		LogTrace(L"(%s)\n", it->name.c_str());
		Item svc(Item::start(m_filter->get_read_manager(), it->name.c_str()));
		if (m_wait_state) {
			svc.wait_state(State::STARTED, m_wait_timout);
		}
		*it = Info(it->name.c_str(), svc);
		notify_changed();
	}

	void Enum::stop(iterator it)
	{
		CRT_ASSERT(it != end());
		LogTrace(L"(%s)\n", it->name.c_str());
		Item svc(Item::stop(m_filter->get_read_manager(), it->name.c_str()));
		if (m_wait_state) {
			svc.wait_state(State::STOPPED, m_wait_timout);
		}
		*it = Info(it->name.c_str(), svc);
		notify_changed();
	}

	void Enum::restart(iterator it)
	{
		CRT_ASSERT(it != end());
		LogTrace(L"(%s)\n", it->name.c_str());
		Item svc(Item::restart(m_filter->get_read_manager(), it->name.c_str()));
		if (m_wait_state) {
			svc.wait_state(State::STARTED, m_wait_timout);
		}
		*it = Info(it->name.c_str(), svc);
		notify_changed();
	}

	void Enum::contin(iterator it)
	{
		CRT_ASSERT(it != end());
		LogTrace(L"(%s)\n", it->name.c_str());
		Item svc(Item::contin(m_filter->get_read_manager(), it->name.c_str()));
		if (m_wait_state) {
			svc.wait_state(State::STARTED, m_wait_timout);
		}
		*it = Info(it->name.c_str(), svc);
		notify_changed();
	}

	void Enum::pause(iterator it)
	{
		CRT_ASSERT(it != end());
		LogTrace(L"(%s)\n", it->name.c_str());
		Item svc(Item::pause(m_filter->get_read_manager(), it->name.c_str()));
		if (m_wait_state) {
			svc.wait_state(State::PAUSED, m_wait_timout);
		}
		*it = Info(it->name.c_str(), svc);
		notify_changed();
	}

	void Enum::set_config(iterator it, const ConfigRequest& request)
	{
		CRT_ASSERT(it != end());
		LogTrace(L"(%s)\n", it->name.c_str());
		request.log();
		*it = Info(it->name.c_str(), Item::set_config(m_filter->get_read_manager(), it->name.c_str(), request));
		notify_changed();
	}

	void Enum::start_batch()
	{
		LogTraceLn();
		m_batch_started = true;
	}

	void Enum::notify_changed()
	{
		set_changed(true);
		if (!m_batch_started) {
			LogTraceLn();
			notify_observers(sync::create_message(sync::MessageI::SYSTEM));
		}
	}

	void Enum::stop_batch()
	{
		LogTraceLn();
		m_batch_started = false;
		notify_observers(sync::create_message(sync::MessageI::SYSTEM));
	}

	void Enum::set_wait_state(bool new_state)
	{
		m_wait_state = new_state;
	}

	bool Enum::get_wait_state() const
	{
		return m_wait_state;
	}

	void Enum::set_wait_timeout(size_t timeout_msec)
	{
		m_wait_timout = timeout_msec;
	}

	size_t Enum::get_wait_timeout() const
	{
		return m_wait_timout;
	}
}
