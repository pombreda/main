#ifndef BASIS_SYS_SYNC_QUEUE_HPP_
#define BASIS_SYS_SYNC_QUEUE_HPP_

namespace sync
{
	class Queue_i
	{
	public:
		virtual ~Queue_i() = default;

		virtual void put_message(const Message& message) = 0;

		virtual WaitResult_t get_message(Message& message, Timeout_t timeout_msec = WAIT_FOREVER) = 0;

		virtual bool empty() const noexcept = 0;
	};

	using Queue = simstd::shared_ptr<Queue_i>;

	Queue create_queue(const wchar_t* name = EMPTY_STR);
}

#endif
