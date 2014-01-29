﻿#include <system/configure.hpp>
#include <system/sync.hpp>

namespace sync {

	ScopeGuard SyncUnit_i::lock_scope()
	{
		return ScopeGuard(this, false);
	}

	ScopeGuard SyncUnit_i::lock_scope_read()
	{
		return ScopeGuard(this, true);
	}

	void SyncUnit_i::destroy() const
	{
		delete this;
	}

}