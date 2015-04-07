#ifndef BASIS_SYS_THREAD_ROUTINE_HPP_
#define BASIS_SYS_THREAD_ROUTINE_HPP_

namespace thread
{
	struct Routine
	{
		static DWORD WINAPI run_thread(void* routine);
		static VOID WINAPI alert_thread(ULONG_PTR routine);

	public:
		Routine();

		virtual ~Routine();

		virtual void alert(void* data);

		virtual ssize_t run(void* data);

		virtual void* get_parameter();

		virtual void put_message(const sync::Message& message);
	};

//	template <typename Type, size_t (Type::*mem_func)(void *)>
//	DWORD WINAPI member_thunk(void * ptr)
//	{ // http://www.rsdn.ru/Forum/Message.aspx?mid=753888&only=1
//		return (static_cast<Type*>(ptr)->*mem_func)(nullptr);
//	}
//
//	template <typename Type, void (Type::*mem_func)(void *)>
//	VOID WINAPI apc_thunk(ULONG_PTR ptr)
//	{
//		(((Type*)(ptr))->*mem_func)(nullptr);
//	}
}

#endif
