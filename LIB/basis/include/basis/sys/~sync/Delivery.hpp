#ifndef BASIS_SYS_SYNC_DELIVERY_HPP_
#define BASIS_SYS_SYNC_DELIVERY_HPP_

namespace sync
{
	namespace delivery
	{
		using SubscribtionId = ssize_t;
		using filter_type = bool (*)(const Message& message);

		SubscribtionId subscribe(const Queue& queue, message::param_type type, message::param_type mask = message::MASK_ALL_TYPES, filter_type filter = nullptr);

		void unsubscribe(SubscribtionId id);

		void unsubscribe(const Queue& queue);

		void send_round(const Message& message);
	}
}

#endif
