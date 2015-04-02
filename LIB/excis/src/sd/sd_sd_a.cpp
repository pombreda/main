#include <basis/sys/logger.hpp>
#include <excis/sd.hpp>
#include <excis/exception.hpp>

namespace Ext {

	WinSD::WinSD(WinSD && rhs):
		m_sd(nullptr)
	{
		LogTraceLn();
		this->swap(rhs);
	}

	WinSD & WinSD::operator = (WinSD && rhs) {
		LogTraceLn();
		if (this != &rhs)
			this->swap(rhs);
//			WinSD(simstd::move(rhs)).swap(*this);
		return *this;
	}

	void WinSD::swap(WinSD & rhs) {
		LogTraceLn();
		using simstd::swap;
		swap(m_sd, rhs.m_sd);
	}

}
