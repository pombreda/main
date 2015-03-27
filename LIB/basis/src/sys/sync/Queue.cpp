#include <basis/sys/sync.hpp>
#include <basis/sys/logger.hpp>

#include <basis/simstd/vector>

namespace sync
{
	LogRegisterLocal(L"sync");

	class QueueImpl: public Queue_i, private CriticalSection, private Semaphore, private simstd::vector<Message>
	{ // deque would bring exceptions dependence
		using base_type = simstd::vector<Message>;

	public:
		QueueImpl(const wchar_t* name);

		void put_message(const value_type& message) override;

		WaitResult_t get_message(value_type& message, Timeout_t timeout_msec) override;

		bool empty() const noexcept override;
	};

	QueueImpl::QueueImpl(const wchar_t* name)
	{
		LogTraceObjFormat(L"(%s)\n", name);
	}

	void QueueImpl::put_message(const value_type& message)
	{
		LogNoise(L"Message(type: %Id, code: %Id, param: %Id, data: %p)\n", message.get_type(), message.get_code(), message.get_param(), message.get_data());
		CriticalSection::lock();
		emplace_back(message);
		CriticalSection::unlock();
		Semaphore::unlock();
	}

	WaitResult_t QueueImpl::get_message(value_type& message, Timeout_t timeout_msec)
	{
		LogNoise(L"(wait: %Id)\n", timeout_msec);
		auto waitResult = Semaphore::try_lock_ex(timeout_msec);
		if (waitResult == WaitResult_t::SUCCESS) {
			CriticalSection::lock();
			message = front();
//			pop_front();
			erase(begin());
			CriticalSection::unlock();
		}
		LogAttenIf(waitResult != WaitResult_t::SUCCESS, L"ret: '%s'\n", totext::c_str(waitResult));
		LogNoiseIf(waitResult == WaitResult_t::SUCCESS, L"ret: '%s' Message(type: %Id, code: %Id, param: %Id, data: %p)\n", totext::c_str(waitResult), message.get_type(), message.get_code(), message.get_param(), message.get_data());
		return waitResult;
	}

	bool QueueImpl::empty() const noexcept
	{
		CriticalSection::lock();
		bool ret = base_type::empty();
		CriticalSection::unlock();
		return ret;
	}

	Queue create_queue(const wchar_t* name)
	{
		LogNoise(L"('%s')\n", name);
		return simstd::make_shared<QueueImpl>(name);
	}
}
