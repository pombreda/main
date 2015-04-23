#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/thread.hpp>

#include <basis/simstd/algorithm>
#include <basis/simstd/vector>
#include <basis/simstd/utility>
#include <basis/simstd/mutex>

namespace sync
{
	struct DeliveryMapping
	{
		DeliveryMapping(const Queue& queue, message::param_type type, message::param_type mask, delivery::filter_type filter):
			queue(queue),
			type(type),
			mask(mask),
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
			return (type & mask) && (message->get_type() & mask);
		}

		bool check_filter(const Message& message) const
		{
			return !filter || filter(message);
		}

		Queue queue;
		message::param_type type;
		message::param_type mask;
		delivery::filter_type filter;
	};

	using dm_t = simstd::pair<delivery::SubscribtionId, DeliveryMapping>;

	struct DeliveryImpl: public thread::Routine, private CriticalSection
	{
		static DeliveryImpl& inst();

		~DeliveryImpl();

		DeliveryImpl();

		delivery::SubscribtionId subscribe(const Queue& queue, message::param_type type, message::param_type mask, delivery::filter_type filter);

		bool unsubscribe(delivery::SubscribtionId id);
		bool unsubscribe(const Queue& queue);

		void send_round(const Message& message) const;

		ssize_t run(void* data) override;

	private:
		DeliveryImpl(const DeliveryImpl&) = delete;
		DeliveryImpl& operator =(const DeliveryImpl&) = delete;

		delivery::SubscribtionId _get_next_id() noexcept {return ++id_generator;}

		delivery::SubscribtionId id_generator = 0;
		simstd::vector<dm_t> subscriptions;
		Queue queue;
		thread::Unit thread;
	};

	DeliveryImpl& DeliveryImpl::inst()
	{
		static DeliveryImpl instance;
		return instance;
	}

	DeliveryImpl::~DeliveryImpl()
	{
		LogTraceObj(L"begin");
		queue->put_message(message::create(message::SYSTEM, message::SYSTEM_THREAD_STOP));
		thread.wait();
		LogTraceObj(L"end");
	}

	DeliveryImpl::DeliveryImpl() :
		queue(create_queue(L"sync::Delivery queue")),
		thread(this, true)
	{
		LogTraceObjLn();
		thread.resume();
	}

	delivery::SubscribtionId DeliveryImpl::subscribe(const Queue& queue, message::param_type type, message::param_type mask, delivery::filter_type filter)
	{
		simstd::lock_guard<CriticalSection> guard(*this);
		auto id = _get_next_id();
		subscriptions.emplace_back(id, DeliveryMapping(queue, type, mask, filter));
		LogTraceObj(L"('%s', 0x%IX, 0x%IX) -> %Id", queue->get_name(), type, mask, id);
		return id;
	}

	bool DeliveryImpl::unsubscribe(delivery::SubscribtionId id)
	{
		bool ret = false;
		simstd::lock_guard<CriticalSection> guard(*this);
		auto it = simstd::find_if(subscriptions.begin(), subscriptions.end(), [&](const dm_t& item) {return item.first == id;});
		if (it != subscriptions.end()) {
			subscriptions.erase(it);
			ret = true;
		}
		LogTraceObj(L"(%Id) -> %d", queue->get_name(), id, ret);
		return ret;
	}

	bool DeliveryImpl::unsubscribe(const Queue& queue)
	{
		simstd::lock_guard<CriticalSection> guard(*this);
		auto old_size = subscriptions.size();
		subscriptions.erase(simstd::remove_if(subscriptions.begin(), subscriptions.end(),  [&](const dm_t& item) {return item.second == queue;}), subscriptions.end());
		bool ret = old_size != subscriptions.size();
		LogTraceObj(L"('%s') -> %d", queue->get_name(), queue->get_name(), ret);
		return ret;
	}

	void DeliveryImpl::send_round(const Message& message) const
	{
		queue->put_message(message);
	}

	ssize_t DeliveryImpl::run(void* /*data*/)
	{
		while (true) {
			Message msg;
			auto waitResult = queue->get_message(msg);
			if (waitResult == WaitResult_t::SUCCESS) {
				if (msg->get_type() == message::SYSTEM && msg->get_a() == message::SYSTEM_THREAD_STOP)
					break;
				simstd::lock_guard<CriticalSection> guard(*this);
				LogTraceObj(L"[size: %Iu]\n", subscriptions.size());
				for (const dm_t& item : subscriptions) {
					LogTraceObjLn();
					item.second(msg);
				}
			} else {
				break;
			}
		}
		return 0;
	}
}

namespace sync
{
	LogRegisterLocal(L"message");

	namespace delivery
	{
		SubscribtionId subscribe(const Queue& queue, message::param_type type, message::param_type mask, filter_type filter)
		{
			LogTraceLn();
			return DeliveryImpl::inst().subscribe(queue, type, mask, filter);
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
