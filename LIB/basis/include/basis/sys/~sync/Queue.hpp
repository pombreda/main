#ifndef BASIS_SYS_SYNC_QUEUE_HPP_
#define BASIS_SYS_SYNC_QUEUE_HPP_

namespace sync
{
	using Queue = simstd::shared_ptr<queue::Interface>;

	Queue create_queue(const wchar_t* name = EMPTY_STR);

	namespace queue
	{
		class Interface
		{
		public:
			virtual ~Interface() = default;

			virtual const wchar_t* get_name() const noexcept = 0;

			virtual void put_message(const Message& message) = 0;

			virtual WaitResult_t get_message(Message& message, int64_t timeout_ms = TIMEOUT_INFINITE) = 0;

			virtual bool empty() const noexcept = 0;
		};
	}
}

#endif
