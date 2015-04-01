#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/vector>
#include <basis/simstd/utility>
#include <basis/simstd/mutex>

namespace sync
{
	struct DeliveryMapping
	{
		DeliveryMapping(const Queue& queue, MessageI::param_type type_mask, MessageI::param_type code_mask, delivery::filter_type filter):
			queue(queue),
			type_mask(type_mask),
			code_mask(code_mask),
			filter(filter)
		{
		}

		bool operator ()(const Message& message) const
		{
			if (check_mask(message) && check_filter(message))
			{
				queue->put_message(message);
				return true;
			}
			return false;
		}

		bool operator ==(const Queue& queue) const
		{
			return this->queue == queue;
		}

	private:
		bool check_mask(const Message& message) const
		{
			return (type_mask & message->get_type()) && (code_mask & message->get_a());
		}

		bool check_filter(const Message& message) const
		{
			return !filter || filter(message);
		}

		Queue queue;
		MessageI::param_type type_mask;
		MessageI::param_type code_mask;
		delivery::filter_type filter;
	};

	using dm_t = simstd::pair<delivery::SubscribtionId, DeliveryMapping>;

	struct DeliveryImpl: private simstd::vector<dm_t>, private CriticalSection
	{
		static DeliveryImpl& inst();

		delivery::SubscribtionId subscribe(const Queue& queue, MessageI::param_type type_mask, MessageI::param_type code_mask, delivery::filter_type filter);

		void unsubscribe(delivery::SubscribtionId id);
		void unsubscribe(const Queue& queue);

		void send_round(const Message& message) const;

	private:
		delivery::SubscribtionId _get_next_id() noexcept {return ++id_generator;}

		mutable CriticalSection cs;
		delivery::SubscribtionId id_generator = 0;
	};

	DeliveryImpl& DeliveryImpl::inst()
	{
		static DeliveryImpl instance;
		return instance;
	}

	delivery::SubscribtionId DeliveryImpl::subscribe(const Queue& queue, MessageI::param_type type_mask, MessageI::param_type code_mask, delivery::filter_type filter)
	{
		simstd::lock_guard<CriticalSection> guard(*this);
		auto id = _get_next_id();
		emplace_back(id, DeliveryMapping(queue, type_mask, code_mask, filter));
		return id;
	}

	void DeliveryImpl::unsubscribe(delivery::SubscribtionId id)
	{
		simstd::lock_guard<CriticalSection> guard(cs);
		auto it = simstd::find_if(begin(), end(), [&](dm_t const& item) {return item.first == id;});
		if (it != end())
			erase(it);
	}

	void DeliveryImpl::unsubscribe(const Queue& queue)
	{
		simstd::lock_guard<CriticalSection> guard(cs);
		for (auto it = rbegin(); it != rend(); ++it) {
			if (it->second == queue)
				erase(it.base());
		}
	}

	void DeliveryImpl::send_round(const Message& message) const
	{
		simstd::lock_guard<CriticalSection> guard(cs);
//		simstd::for_each(begin(), end(), [&](dm_t const& item) {
//			item.second(message);
//		});
		for (const dm_t& item : *this) {
			item.second(message);
		}
	}
}

namespace sync
{
	LogRegisterLocal(L"message");

	namespace delivery
	{
		SubscribtionId subscribe(const Queue& queue, MessageI::param_type type_mask, MessageI::param_type code_mask, filter_type filter)
		{
			LogTraceLn();
			return DeliveryImpl::inst().subscribe(queue, type_mask, code_mask, filter);
		}

		void unsubscribe(SubscribtionId id)
		{
			LogTraceLn();
			DeliveryImpl::inst().unsubscribe(id);
		}

		void unsubscribe(const Queue& queue)
		{
			LogTraceLn();
			DeliveryImpl::inst().unsubscribe(queue);
		}

		void send_round(const Message& message)
		{
			LogTraceLn();
			DeliveryImpl::inst().send_round(message);
		}
	}
}
