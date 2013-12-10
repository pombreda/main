﻿#include <libbase/lock.hpp>

#include <algorithm>

namespace Lock {

	ScopeGuard::~ScopeGuard()
	{
		if (m_unit)
			m_unit->unlock();
	}

	ScopeGuard::ScopeGuard() :
		m_unit(nullptr)
	{
	}

	ScopeGuard::ScopeGuard(SyncUnit_i * unit, bool readOnly) :
		m_unit(unit)
	{
		if (m_unit)
			m_unit->lock(readOnly);
	}

	ScopeGuard::ScopeGuard(ScopeGuard && right) :
		m_unit(nullptr)
	{
		swap(right);
	}

	ScopeGuard & ScopeGuard::operator =(ScopeGuard && right)
	{
		ScopeGuard(std::move(right)).swap(*this);
		return *this;
	}

	void ScopeGuard::swap(ScopeGuard & right)
	{
		using std::swap;
		swap(m_unit, right.m_unit);
	}

}
