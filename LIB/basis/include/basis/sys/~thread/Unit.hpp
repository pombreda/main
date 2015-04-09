#ifndef BASIS_SYS_THREAD_UNIT_HPP_
#define BASIS_SYS_THREAD_UNIT_HPP_

namespace thread
{
	struct Unit: private pattern::Uncopyable
	{
		using handle_type = HANDLE;

	public:
		~Unit() noexcept;

		Unit(Routine* routine, bool suspended = false, size_t stack_size = 0);
		Unit(Routine* routine, void* data, bool suspended = false, size_t stack_size = 0);
		Unit(Unit&& right);

		Unit& operator =(Unit&& right);

		void swap(Unit& right) noexcept;

		bool is_valid() const;

		bool alert();
		bool alert(void* data);

		bool set_priority(Priority prio);
		bool set_io_priority(IoPriority prio);

		size_t get_exitcode() const;

		Id get_id() const;

		handle_type get_handle() const;

		Priority get_priority() const;
		IoPriority get_io_priority() const;

		Routine* get_routine() const;

		bool suspend() const;
		bool resume() const;

		sync::WaitResult_t wait(int64_t timeout = sync::TIMEOUT_INFINITE) const;

	private:
		Routine* m_routine;
		handle_type m_handle;
		Id m_id;
	};
}

#endif
