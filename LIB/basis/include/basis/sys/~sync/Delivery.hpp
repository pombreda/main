#ifndef BASIS_SYS_SYNC_DELIVERY_HPP_
#define BASIS_SYS_SYNC_DELIVERY_HPP_

#include <basis/sys/sync.hpp>

namespace sync {

	namespace Delivery {
		typedef ssize_t SubscribtionId;

		typedef bool (*filter_t)(const Message & message);

		SubscribtionId Subscribe(Queue_i * queue, Message::type_t type_mask = Message::MASK_ALL_TYPES, Message::code_t code_mask = Message::MASK_ALL_CODES, filter_t filter = nullptr);

		void Unsubscribe(SubscribtionId id);

		void Unsubscribe(const Queue_i * queue);

		void SendRound(const Message & message);

		class Subscriber: private Queue_i {
		public:
			Subscriber(Message::type_t type_mask = Message::MASK_ALL_TYPES, Message::code_t code_mask = Message::MASK_ALL_CODES, filter_t filter = nullptr)
			{
				Subscribe(this, type_mask, code_mask, filter);
			}

			~Subscriber()
			{
				Unsubscribe(this);
			}
		};
	}

}

#endif
