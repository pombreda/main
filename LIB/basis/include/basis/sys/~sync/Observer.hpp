#ifndef BASIS_SYS_SYNC_OBSERVER_HPP_
#define BASIS_SYS_SYNC_OBSERVER_HPP_

namespace sync
{
	using MessageManager = MessageManagerI*;

	class MessageManagerI
	{
	public:
		static MessageManager get_default();

		virtual ~MessageManagerI() = default;

		virtual void register_observer(const Observable* subject, Observer* observer) = 0;

		virtual void unregister_observer(const Observable* subject, const Observer* observer) = 0;

		virtual void unregister_observable(const Observable* subject) = 0;

		virtual void unregister_observer(const Observer* observer) = 0;

		virtual void notify(const Observable* subject, const Message& message) const = 0;
	};

	///================================================================================== Observable
	class Observable
	{
	public:
		virtual ~Observable();

		Observable();

		void register_observer(Observer* observer) const;

		void unregister_observer(Observer* observer) const;

		void notify_observers(const Message& message) const;

	protected:
		void set_changed(bool changed) const;

		bool get_changed() const;

	private:
		mutable MessageManager m_manager;
		mutable bool m_changed;
	};

	///==================================================================================== Observer
	class Observer
	{
	public:
		virtual ~Observer();

		virtual void notify(const Message& message) = 0;

		Observer();

		Observer(const Observable* subject);

	private:
		MessageManager m_manager;
	};
}

#endif
