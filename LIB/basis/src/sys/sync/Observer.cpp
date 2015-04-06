#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

namespace sync
{
	Observer::~Observer()
	{
		LogTraceObjLn();
		m_manager->unregister_observer(this);
	}

	Observer::Observer() :
		m_manager(MessageManagerI::get_default())
	{
		CRT_ASSERT(m_manager);
		LogTraceObjLn();
	}

	Observer::Observer(const Observable* subject):
		Observer()
	{
		CRT_ASSERT(subject);
		LogTraceObjLn();
		subject->register_observer(this);
	}
}

