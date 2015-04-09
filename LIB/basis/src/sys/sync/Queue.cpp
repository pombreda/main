#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

#include <basis/simstd/mutex>
#include <basis/simstd/string>
#include <basis/simstd/vector>

namespace sync
{
	LogRegisterLocal(L"sync");

	class QueueImpl: public queue::Interface, private Semaphore, private simstd::vector<Message>
	{ // std::deque would bring exceptions dependence
		using base_type = simstd::vector<Message>;

	public:
		QueueImpl(const wchar_t* name);

		const wchar_t* get_name() const noexcept override;

		void put_message(const Message& message) override;

		WaitResult_t get_message(Message& message, int64_t timeout_msec) override;

		bool empty() const noexcept override;

	private:
		mutable CriticalSection cs;
		simstd::wstring name;
	};

	QueueImpl::QueueImpl(const wchar_t* _name):
		name(_name)
	{
		LogTraceObj(L"(%s)\n", _name);
	}

	const wchar_t* QueueImpl::get_name() const noexcept
	{
		return name.c_str();
	}

	void QueueImpl::put_message(const Message& message)
	{
		LogTraceObj(L"['%s'] Message(0x%IX(%Iu, %Iu, %Iu))\n", name.c_str(), message->get_type(), message->get_a(), message->get_b(), message->get_c());
		{
			simstd::lock_guard<CriticalSection> guard(cs);
			emplace_back(message->clone());
		}
		Semaphore::unlock();
	}

	WaitResult_t QueueImpl::get_message(Message& message, int64_t timeout_msec)
	{
		LogTraceObj(L"['%s'] (%Id)\n", name.c_str(), timeout_msec);
		auto waitResult = Semaphore::try_lock_ex(timeout_msec);
		if (waitResult == WaitResult_t::SUCCESS) {
			simstd::lock_guard<CriticalSection> guard(cs);
			message = simstd::move(front());
//			pop_front();
			erase(begin());
		}
		LogAttenIf(waitResult != WaitResult_t::SUCCESS, L"['%s'] -> '%s'\n", name.c_str(), totext::c_str(waitResult));
		LogDebugIf(waitResult == WaitResult_t::SUCCESS, L"['%s'] -> '%s' Message(0x%IX(%Iu, %Iu, %Iu))\n", name.c_str(), totext::c_str(waitResult), message->get_type(), message->get_a(), message->get_b(), message->get_c());
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
