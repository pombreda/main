#ifndef BASIS_SYS_SYNC_QUEUE_HPP_
#define BASIS_SYS_SYNC_QUEUE_HPP_

namespace sync
{
	using Queue = simstd::shared_ptr<QueueI>;

	Queue create_queue(const wchar_t* name = EMPTY_STR);

	class QueueI
	{
	public:
		virtual ~QueueI() = default;

		virtual void put_message(const Message& message) = 0;

		virtual WaitResult_t get_message(Message& message, size_t timeout_ms = WAIT_FOREVER) = 0;

		virtual bool empty() const noexcept = 0;
	};
}

#endif
