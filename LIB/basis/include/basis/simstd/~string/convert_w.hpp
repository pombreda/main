﻿#ifndef BASIS_STRING_CONVERT_W_HPP_
#define BASIS_STRING_CONVERT_W_HPP_

#include <basis/simstd/string>

namespace simstd {

	wstring to_wstring(int value);

	wstring to_wstring(long value);

	wstring to_wstring(long long value);

	wstring to_wstring(unsigned value);

	wstring to_wstring(unsigned long value);

	wstring to_wstring(unsigned long long value);

	wstring to_wstring(float value);

	wstring to_wstring(double value);

	wstring to_wstring(long double value);

	int stoi(const wstring& str, size_t *pos = nullptr, int base = 10);

	long stol(const wstring& str, size_t *pos = nullptr, int base = 10);

	long long stoll(const wstring& str, size_t *pos = nullptr, int base = 10);

	unsigned long stoul(const wstring& str, size_t *pos = nullptr, int base = 10);

	unsigned long long stoull(const wstring& str, size_t *pos = nullptr, int base = 10);

	float stof(const wstring& str, size_t *pos = nullptr);

	double stod(const wstring& str, size_t *pos = nullptr);

	long double stold(const wstring& str, size_t *pos = nullptr);
}

#endif
