#ifndef BASIS_SYS_THREAD_UNIT_HPP_
#define BASIS_SYS_THREAD_UNIT_HPP_

namespace thread
{
	class UnitIface
	{
		struct native_impl_type;

	public:
		using handle_type = native_impl_type*;

		virtual ~UnitIface() = default;

		virtual bool is_valid() const noexcept = 0;

		virtual bool alert() noexcept = 0;
		virtual bool alert(void* data) noexcept = 0;

		virtual bool set_priority(Priority prio) noexcept = 0;
		virtual bool set_io_priority(IoPriority prio) noexcept = 0;

		virtual size_t get_exitcode() const noexcept = 0;

		virtual Id get_id() const noexcept = 0;

		virtual handle_type get_handle() const noexcept = 0;

		virtual Priority get_priority() const noexcept = 0;
		virtual IoPriority get_io_priority() const noexcept = 0;

		virtual Routine* get_routine() const noexcept = 0;

		virtual const wchar_t* get_name() const noexcept = 0;

		virtual bool suspend() const noexcept = 0;
		virtual bool resume() const noexcept = 0;

		virtual sync::WaitResult_t wait(int64_t timeout = sync::TIMEOUT_INFINITE) const noexcept = 0;
	};

	using Unit = simstd::unique_ptr<UnitIface>;

	Unit create(const wchar_t* name, Routine* routine, bool suspended = false, size_t stack_size = 0);
	Unit create(const wchar_t* name, Routine* routine, void* data, bool suspended = false, size_t stack_size = 0);
}

#endif
