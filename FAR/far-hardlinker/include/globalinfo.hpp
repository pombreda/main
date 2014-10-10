﻿/**
	hardlinker: Search duplicates and make hardlinks
	FAR3-lua plugin

	© 2014 Andrew Grechkin

	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#ifndef _GLOBALINFO_HPP_
#define _GLOBALINFO_HPP_

#include <libfar3/globalinfo_i.hpp>

#include <basis/ext/pattern.hpp>
#include <basis/simstd/string>

struct FarGlobalInfo: public Far::GlobalInfo_i, private pattern::Uncopyable {
	~FarGlobalInfo();

	FarGlobalInfo();

	PCWSTR get_author() const override;

	PCWSTR get_description() const override;

	const GUID * get_guid() const override;

	PCWSTR get_title() const override;

	VersionInfo get_version() const override;

	VersionInfo get_min_version() const override;

	Far::Plugin_i * CreatePlugin(const PluginStartupInfo * Info) const override;

	void load_settings();

	void save_settings() const;

	WCHAR prefix[32];
	ssize_t m_cbOperation;
	int64_t m_cbMask;
	int64_t m_cbDoHardlink;
	int64_t m_cbDoRecursive;
	int64_t m_cbAskConfirmation;
	int64_t m_cbEuristic;
	int64_t m_cbRestrictionFileTime;
	int64_t m_cbRestrictionFileAttributes;
	int64_t m_cbFilterFileSize;
	int64_t m_cbFilterFileReadOnly;
	int64_t m_cbFilterFileHidden;
	int64_t m_cbFilterFileSystem;
	int64_t m_cbFilterFileLink;
	int64_t m_cbFilterDirReadOnly;
	int64_t m_cbFilterDirHidden;
	int64_t m_cbFilterDirSystem;
	int64_t m_cbFilterDirLink;
	ustring m_edMask;
};

FarGlobalInfo * get_global_info();

#endif
