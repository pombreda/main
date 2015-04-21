/**
 ontop: Always on top
 FAR3 plugin
 Switch between "always on top" state on/off

 © 2013 Andrew Grechkin

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

#include <far3/plugin.hpp>
#include <basis/sys/crt.hpp>
#include <basis/sys/logger.hpp>

namespace
{
	void setup_logger()
	{
		LogSetOptions(L"logger:///default?level=w;prefix=f;target=fo(far-ontop.log)");
	}
}

///========================================================================================== Export
/// GlobalInfo
void WINAPI GetGlobalInfoW(GlobalInfo * Info)
{
	setup_logger();

	LogTraceLn();
	far3::helper_t::inst().init(new FarGlobalInfo);
	get_global_info()->GetGlobalInfoW(Info);
}

void WINAPI SetStartupInfoW(const PluginStartupInfo * Info)
{
	LogTraceLn();
	get_global_info()->SetStartupInfoW(Info);
	get_global_info()->load_settings();
}

intptr_t WINAPI ConfigureW(const ConfigureInfo * Info)
{
	LogTraceLn();
	return get_global_info()->ConfigureW(Info);
}

/// Plugin
void WINAPI GetPluginInfoW(PluginInfo * Info)
{
	LogTraceLn();
	far3::helper_t::inst().get_plugin()->GetPluginInfoW(Info);
}

HANDLE WINAPI OpenW(const OpenInfo * Info)
{
	LogTraceLn();
	return far3::helper_t::inst().get_plugin()->OpenW(Info);
}

void WINAPI ExitFARW(const ExitInfo *Info)
{
	LogTraceLn();
	far3::helper_t::inst().get_plugin()->ExitFARW(Info);
}

/// Panel

#ifndef DEBUG

///=================================================================================================
extern "C"
{
	BOOL WINAPI DllMainCRTStartup(HANDLE, DWORD dwReason, PVOID)
	{
		switch (dwReason) {
			case DLL_PROCESS_ATTACH:
				crt::init_atexit();
				break;

			case DLL_PROCESS_DETACH:
				crt::invoke_atexit();
				break;
		}
		return true;
	}

	int atexit(crt::Function pf)
	{
		return crt::atexit(pf);
		return 0;
	}

	void __cxa_pure_virtual(void)
	{
		crt::cxa_pure_virtual();
	}
}

#endif
