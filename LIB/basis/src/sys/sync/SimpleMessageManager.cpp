#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/vector>
#include <basis/simstd/utility>
#include <basis/simstd/mutex>

namespace sync
{
	using Mapping = simstd::pair<const Observable*, Observer*>;

	inline bool operator ==(const Mapping& left, const Observer* right)
			{
		return left.second == right;
			}

	struct MappingLess
	{
		bool operator ()(const Mapping& left, const Observable* right)
		{
			return left.first < right;
		}

		bool operator ()(const Observable* left, const Mapping& right)
		{
			return left < right.first;
		}
	};

	class SimpleMessageManager:
		public MessageManagerI,
		private simstd::vector<Mapping>
	{
	public:
		void register_observer(const Observable* subject, Observer* observer) override;

		void unregister_observer(const Observable* subject, const Observer* observer) override;

		void unregister_observable(const Observable* subject) override;

		void unregister_observer(const Observer* observer) override;

		void notify(const Observable* subject, const Message& event) const override;

	private:
		mutable CriticalSection cs;
	};

	void SimpleMessageManager::register_observer(const Observable* subject, Observer* observer)
	{
		LogTrace2(L"(%p, %p)\n", subject, observer);
		auto guard(simstd::auto_lock(cs));
		emplace(simstd::upper_bound(begin(), end(), subject, MappingLess()), subject, observer);
	}

	void SimpleMessageManager::unregister_observer(const Observable* subject, const Observer* observer)
	{
		LogTrace2(L"(%p, %p)\n", subject, observer);
		auto guard(simstd::auto_lock(cs));
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		erase(remove(range.first, range.second, observer), range.second);
	}

	void SimpleMessageManager::unregister_observable(const Observable* subject)
	{
		LogTrace2(L"(%p)\n", subject);
		auto guard(simstd::auto_lock(cs));
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		erase(range.first, range.second);
	}

	void SimpleMessageManager::unregister_observer(const Observer* observer)
	{
		LogTrace2(L"(%p)\n", observer);
		auto guard(simstd::auto_lock(cs));
		erase(remove(begin(), end(), observer), end());
	}

	void SimpleMessageManager::notify(const Observable* subject, const Message& event) const
	{
		LogTrace2(L"(%p, 0x%IX(%Iu, %Iu, %Iu))\n", subject, event->get_type(), event->get_a(), event->get_b(), event->get_c());
		auto guard(simstd::auto_lock(cs));
		auto range = simstd::equal_range(begin(), end(), subject, MappingLess());
		simstd::for_each(range.first, range.second, [&event](const Mapping& pair) {
			pair.second->notify(event);
		});
	}

	MessageManager MessageManagerI::get_default()
	{
		static SimpleMessageManager ret;
		return &ret;
	}
}
