#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

#include <basis/simstd/mutex>
#include <basis/simstd/vector>

namespace sync
{
	LogRegisterLocal(L"sync");

	class QueueImpl: public QueueI, private Semaphore, private simstd::vector<Message>
	{ // deque would bring exceptions dependence
		using base_type = simstd::vector<Message>;

	public:
		QueueImpl(const wchar_t* name);

		void put_message(const Message& message) override;

		WaitResult_t get_message(Message& message, size_t timeout_msec) override;

		bool empty() const noexcept override;

	private:
		mutable CriticalSection cs;
	};

	QueueImpl::QueueImpl(const wchar_t* name)
	{
		LogTraceObj(L"(%s)\n", name);
	}

	void QueueImpl::put_message(const Message& message)
	{
		LogTraceObj(L"Message(0x%IX(%Iu, %Iu, %Iu))\n", message->get_type(), message->get_a(), message->get_b(), message->get_c());
		{
			simstd::lock_guard<CriticalSection> guard(cs);
			emplace_back(message->clone());
		}
		Semaphore::unlock();
	}

	WaitResult_t QueueImpl::get_message(Message& message, size_t timeout_msec)
	{
		LogTraceObj(L"(%Id)\n", timeout_msec);
		auto waitResult = Semaphore::try_lock_ex(timeout_msec);
		if (waitResult == WaitResult_t::SUCCESS) {
			simstd::lock_guard<CriticalSection> guard(cs);
			message = simstd::move(front());
//			pop_front();
			erase(begin());
		}
		LogAttenIf(waitResult != WaitResult_t::SUCCESS, L"-> '%s'\n", totext::c_str(waitResult));
		LogDebugIf(waitResult == WaitResult_t::SUCCESS, L"-> '%s' Message(0x%IX(%Iu, %Iu, %Iu))\n", totext::c_str(waitResult), message->get_type(), message->get_a(), message->get_b(), message->get_c());
		return waitResult;
	}

	bool QueueImpl::empty() const noexcept
	{
		simstd::lock_guard<CriticalSection> guard(cs);
		return base_type::empty();
	}

	Queue create_queue(const wchar_t* name)
	{
		LogTrace(L"('%s')\n", name);
		return simstd::make_shared<QueueImpl>(name);
	}
}
