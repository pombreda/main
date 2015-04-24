#ifndef BASIS_SYS_THREAD_UNITIMPL_HPP_
#define BASIS_SYS_THREAD_UNITIMPL_HPP_

#include <basis/simstd/string>

namespace thread
{
	struct UnitImpl: public UnitIface
	{
		~UnitImpl() noexcept;

		UnitImpl(const wchar_t* name, Routine* routine, bool suspended, size_t stack_size) noexcept;
		UnitImpl(const wchar_t* name, Routine* routine, void* data, bool suspended, size_t stack_size) noexcept;

		bool is_valid() const noexcept override;

		bool alert() noexcept override;
		bool alert(void* data) noexcept override;

		bool set_priority(Priority prio) noexcept override;
		bool set_io_priority(IoPriority prio) noexcept override;

		size_t get_exitcode() const noexcept override;

		Id get_id() const noexcept override;

		handle_type get_handle() const noexcept override;

		Priority get_priority() const noexcept override;
		IoPriority get_io_priority() const noexcept override;

		Routine* get_routine() const noexcept override;

		const wchar_t* get_name() const noexcept override;

		bool suspend() const noexcept override;
		bool resume() const noexcept override;

		sync::WaitResult_t wait(int64_t timeout = sync::TIMEOUT_INFINITE) const noexcept override;

	private:
		Routine* m_routine;
		handle_type m_handle;
		Id m_id;
		simstd::wstring name;
	};
}

#endif
