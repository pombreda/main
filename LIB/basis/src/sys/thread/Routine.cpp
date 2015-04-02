#include <basis/sys/thread.hpp>

namespace thread {

	DWORD WINAPI Routine::run_thread(void* routine)
	{
		LogTrace(L"%p\n", routine);
		Routine * l_routine = reinterpret_cast<Routine*>(routine);
		return l_routine->run(l_routine->get_parameter());
	}

	VOID WINAPI Routine::alert_thread(ULONG_PTR routine)
	{
		LogTrace(L"%p\n", routine);
		reinterpret_cast<Routine*>(routine)->alert(nullptr);
	}

	Routine::Routine()
	{
		LogTraceLn();
	}

	Routine::~Routine()
	{
		LogTraceLn();
	}

	void Routine::alert(void* data)
	{
		UNUSED(data);
		LogTrace(L"data: %p\n", data);
	}

	size_t Routine::run(void* data)
	{
		UNUSED(data);
		LogTrace(L"data: %p\n", data);
		return 0;
	}

	void* Routine::get_parameter()
	{
		return nullptr;
	}

	void Routine::put_message(const sync::Message& message)
	{
		UNUSED(message);
		LogTrace(L"0x%IX(%Iu, %Iu, %Iu)\n", message->get_type(), message->get_a(), message->get_b(), message->get_c());
	}
}
