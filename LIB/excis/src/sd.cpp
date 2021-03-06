﻿#include <excis/sd.hpp>
#include <excis/exception.hpp>
//#include <excis/dacl.hpp>
#include <excis/priv.hpp>
#include <basis/ext/bits.hpp>
//#include <libbase/bit_str.hpp>

#include <sddl.h>

namespace Ext {

	///======================================================================================= WinSD
	WinSD::~WinSD() {
		LogTraceLn();
		free(m_sd);
	}

	PSECURITY_DESCRIPTOR WinSD::alloc(size_t size) {
		return (PSECURITY_DESCRIPTOR)::LocalAlloc(LMEM_ZEROINIT, size);
	}

	void WinSD::free(PSECURITY_DESCRIPTOR & in) {
		if (in) {
			::LocalFree(in);
			in = nullptr;
		}
	}

	bool WinSD::is_valid(PSECURITY_DESCRIPTOR sd) {
		return sd && ::IsValidSecurityDescriptor(sd);
	}

	bool WinSD::is_protected(PSECURITY_DESCRIPTOR sd) {
		return bits::Flags::check(get_control(sd), (WORD)SE_DACL_PROTECTED);
	}

	bool WinSD::is_selfrelative(PSECURITY_DESCRIPTOR sd) {
		return bits::Flags::check(get_control(sd), (WORD)SE_SELF_RELATIVE);
	}

	size_t WinSD::size(PSECURITY_DESCRIPTOR sd) {
		CheckApi(is_valid(sd));
		return ::GetSecurityDescriptorLength(sd);
	}

}
