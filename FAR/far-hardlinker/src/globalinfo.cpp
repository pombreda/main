/**
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

#include <globalinfo.hpp>
#include <farplugin.hpp>
#include <guid.hpp>
#include <lang.hpp>

#include <libfar3/helper.hpp>

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>

#include <version.h>

FarGlobalInfo::FarGlobalInfo()
{
	LogTrace();
	cstr::copy(prefix, L"hardlinker");
	m_cbOperation = 0;
	m_cbMask = 0;
	m_cbDoHardlink = 0;
	m_cbDoRecursive = 1;
	m_cbAskConfirmation = 1;
	m_cbEuristic = 0;
	m_cbRestrictionFileTime = 0;
	m_cbRestrictionFileAttributes = 0;
	m_cbFilterFileSize = 0;
	m_cbFilterFileReadOnly = 0;
	m_cbFilterFileHidden = 0;
	m_cbFilterFileSystem = 0;
	m_cbFilterFileLink = 0;
	m_cbFilterDirReadOnly = 0;
	m_cbFilterDirHidden = 0;
	m_cbFilterDirSystem = 0;
	m_cbFilterDirLink = 0;
	m_edMask = L"*";
}

FarGlobalInfo::~FarGlobalInfo()
{
	LogTrace();
}

PCWSTR FarGlobalInfo::get_author() const
{
	return L"© 2014 Andrew Grechkin";
}

PCWSTR FarGlobalInfo::get_description() const
{
	return L"Search duplicates and make hardlinks";
}

const GUID * FarGlobalInfo::get_guid() const
{
	return &PluginGuid;
}

PCWSTR FarGlobalInfo::get_title() const
{
	return L"hardlinker";
}

VersionInfo FarGlobalInfo::get_version() const
{
	using namespace AutoVersion;
	return MAKEFARVERSION(MAJOR, MINOR, BUILD, FARMANAGERVERSION_BUILD, VS_RELEASE);
}

VersionInfo FarGlobalInfo::get_min_version() const
{
	return MAKEFARVERSION(3, 0, 0, 3000, VS_RELEASE);
}

Far::Plugin_i * FarGlobalInfo::CreatePlugin(const PluginStartupInfo * Info) const
{
	LogTrace();
	Far::Plugin_i * plugin = create_FarPlugin(Info);
	return plugin;
}

void FarGlobalInfo::load_settings()
{
	LogTrace();
}

void FarGlobalInfo::save_settings() const
{
	LogTrace();
}

FarGlobalInfo * get_global_info()
{
	return (FarGlobalInfo*)Far::helper_t::inst().get_global_info();
}