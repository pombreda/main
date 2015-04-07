#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

namespace sync
{
	Observable::~Observable()
	{
		LogTraceObjLn();
		m_manager->unregister_observable(this);
	}

	Observable::Observable() :
		m_manager(MessageManagerI::get_default()),
		m_changed(false)
	{
		CRT_ASSERT(m_manager);
		LogTraceObjLn();
	}

	void Observable::register_observer(Observer* observer) const
	{
		CRT_ASSERT(observer);
		m_manager->register_observer(this, observer);
	}

	void Observable::unregister_observer(Observer* observer) const
	{
		CRT_ASSERT(observer);
		m_manager->unregister_observer(this, observer);
	}

	void Observable::notify_observers(const Message& event) const
	{
		CRT_ASSERT(event);
		if (m_changed) {
			m_manager->notify(this, event);
			m_changed = false;
		}
	}

	void Observable::set_changed(bool changed) const
	{
		m_changed = changed;
	}

	bool Observable::get_changed() const
	{
		return m_changed;
	}
}
