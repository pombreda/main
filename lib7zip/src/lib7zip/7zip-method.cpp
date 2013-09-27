﻿#include <lib7zip/7zip.hpp>
#include <libext/exception.hpp>

namespace SevenZip {
	///====================================================================================== Method
	Method::Method(const Lib & arc_lib, size_t idx)
	{
		Com::PropVariant prop;
		CheckApiError(arc_lib.GetMethodProperty(idx, 0, prop.ref()));
		id = prop.as_uint();
		CheckApiError(arc_lib.GetMethodProperty(idx, 1, prop.ref()));
		name = prop.as_str();
		arc_lib.GetMethodProperty(idx, 2, prop.ref());
		if (prop.vt == VT_BSTR) {
			size_t len = ::SysStringByteLen(prop.bstrVal);
			BYTE* data = reinterpret_cast<BYTE*>(prop.bstrVal);
			start_sign.assign(&data[0], &data[len]);
		};
		arc_lib.GetMethodProperty(idx, 3, prop.ref());
		if (prop.vt == VT_BSTR) {
			size_t len = ::SysStringByteLen(prop.bstrVal);
			BYTE* data = reinterpret_cast<BYTE*>(prop.bstrVal);
			finish_sign.assign(&data[0], &data[len]);
		};
	}

	bool Method::operator <(const Method & rhs) const
	{
		return name < rhs.name;
	}

	bool Method::operator ==(const Method & rhs) const
	{
		return name == rhs.name;
	}

	bool Method::operator !=(const Method & rhs) const
	{
		return name != rhs.name;
	}

	///===================================================================================== Methods
	Methods::Methods()
	{
	}

	void Methods::cache(const Lib & lib)
	{
		UInt32 num_methods = 0;
		CheckCom(lib.GetNumberOfMethods(&num_methods));

		clear();

		for (size_t idx = 0; idx < num_methods; ++idx) {
			emplace_back(lib, idx);
		}
	}
}
