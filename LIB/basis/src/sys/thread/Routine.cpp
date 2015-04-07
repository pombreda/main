#include <basis/sys/thread.hpp>
#include <basis/sys/logger.hpp>

namespace thread
{
	DWORD WINAPI Routine::run_thread(void* routine)
	{
		LogTrace(L"(%p)\n", routine);
		Routine * l_routine = reinterpret_cast<Routine*>(routine);
		return l_routine->run(l_routine->get_parameter());
	}

	VOID WINAPI Routine::alert_thread(ULONG_PTR routine)
	{
		LogTrace(L"(%p)\n", (void*)routine);
		reinterpret_cast<Routine*>(routine)->alert(nullptr);
	}

	Routine::Routine()
	{
		LogTraceObjLn();
	}

	Routine::~Routine()
	{
		LogTraceObjLn();
	}

	void Routine::alert(void* data)
	{
		UNUSED(data);
		LogTraceObj(L"(%p)\n", data);
	}

	ssize_t Routine::run(void* data)
	{
		UNUSED(data);
		LogTraceObj(L"(%p)\n", data);
		return 0;
	}

	void* Routine::get_parameter()
	{
		return nullptr;
	}

	void Routine::put_message(const sync::Message& message)
	{
		UNUSED(message);
		LogTraceObj(L"0x%IX(%Iu, %Iu, %Iu)\n", message->get_type(), message->get_a(), message->get_b(), message->get_c());
	}
}
