#include <basis/sys/traceback.hpp>

#include <basis/simstd/string>

namespace traceback
{
	LazyFrame::LazyFrame(void* address) :
		m_address(address)
	{
	}

	LazyFrame::LazyFrame(LazyFrame&& other) :
		m_address(other.m_address),
		frame(simstd::move(other.frame))
	{
	}

	LazyFrame& LazyFrame::operator =(LazyFrame&& other)
	{
		LazyFrame(simstd::move(other)).swap(*this);
		return *this;
	}

	void LazyFrame::swap(LazyFrame& other)
	{
		using simstd::swap;
		swap(m_address, other.m_address);
		swap(frame, other.frame);
	}

	void* LazyFrame::address() const
	{
		return m_address;
	}

	const ustring& LazyFrame::module() const
	{
		init_data();
		return frame->module();
	}

	const ustring& LazyFrame::file() const
	{
		init_data();
		return frame->file();
	}

	const ustring& LazyFrame::function() const
	{
		init_data();
		return frame->function();
	}

	size_t LazyFrame::line() const
	{
		init_data();
		return frame->line();
	}

	size_t LazyFrame::offset() const
	{
		init_data();
		return frame->offset();
	}

	ustring LazyFrame::to_str() const
	{
		wchar_t buf[MAX_PATH];
		if (file().empty())
			if (function().empty())
				safe_snprintf(buf, lengthof(buf), L"[%s] (%p)", module().c_str(), address());
			else
				safe_snprintf(buf, lengthof(buf), L"[%s] (%p) %s:0x%Ix", module().c_str(), address(), function().c_str(), offset());
		else
			safe_snprintf(buf, lengthof(buf), L"[%s] (%p) %s:0x%Ix {%s:%Iu}", module().c_str(), address(), function().c_str(), offset(), file().c_str(), line());
		return simstd::move(ustring(buf));
	}

	void LazyFrame::init_data() const
	{
		if (!frame)
			frame = read_frame_data(m_address);
	}
}
