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

#include <farplugin.hpp>
#include <globalinfo.hpp>
#include <guid.hpp>
#include <lang.hpp>
#include <FileProcessor.hpp>
#include <global.hpp>
#include <fsys.hpp>

#include <far3/plugin.hpp>
#include <far3/message.hpp>
#include <far3/dialog.hpp>
#include <far3/panel.hpp>

#include <basis/sys/cstr.hpp>
#include <basis/sys/logger.hpp>
#include <basis/sys/fsys.hpp>
#include <basis/sys/path.hpp>

struct FarPlugin: public far3::Plugin_i {
	~FarPlugin();

	FarPlugin(const PluginStartupInfo* info);

	void destroy() const override;

	void GetPluginInfo(PluginInfo* info) override;

	far3::PanelController_i* Open(const OpenInfo* info) override;

private:
	mutable WCHAR menu_item[64];
};

FarPlugin::~FarPlugin()
{
}

FarPlugin::FarPlugin(const PluginStartupInfo * info):
	far3::Plugin_i(info)
{
	LogTraceObjLn();

	global::vars().cryptProvider = crypt::provider();
}

void FarPlugin::destroy() const
{
	delete this;
}

void FarPlugin::GetPluginInfo(PluginInfo * info)
{
	LogTraceObjLn();
	info->Flags = PF_EDITOR | PF_VIEWER | PF_DIALOG;

	static GUID PluginMenuGuids[] = {MenuGuid,};
	static PCWSTR PluginMenuStrings[] = {menu_item,};

	cstr::copy(menu_item, far3::message::get(far3::message::MenuTitle), lengthof(menu_item));

	info->PluginMenu.Guids = PluginMenuGuids;
	info->PluginMenu.Strings = PluginMenuStrings;
	info->PluginMenu.Count = lengthof(PluginMenuStrings);

	info->CommandPrefix = get_global_info()->prefix;
}

namespace far3 {
	class Menu {
	public:
		Menu(const GUID& guid):
			m_guid(guid)
		{
			LogTraceObjLn();
		}

		bool show() {
			LogTraceObjLn();

			static FarMenuItem items[] = {
				{MIF_NONE, L"qwe1", {0, 0}, 0, {0, 0}},
			};

			auto ptr = far3::psi().Menu(get_plugin_guid(), &m_guid, -1, -1, 0, FMENU_NONE, L"menu", nullptr, nullptr, nullptr, nullptr, items, lengthof(items));
			return ptr;
		}

	private:
		const GUID& m_guid;
	};

}

far3::PanelController_i* FarPlugin::Open(const OpenInfo* info)
{
	UNUSED(info);
	LogTraceObjLn();

	using namespace far3;
	auto fgi = get_global_info();
	fgi->load_settings();

	FarListItem cbOperation[] = {
		{0, message::get(lbSearchOnly), {0}},
		{0, message::get(lbHardlinkAuto), {0}},
		{0, message::get(lbHardlinkManual), {0}},
	};

	auto dialog = dialog::create_builder(32, DialogGuid, message::get(message::DlgTitle));
	if (dialog) {
		dialog->add_item(dialog::create_label(txOperation, 16));
		dialog->add_item_after(dialog::create_combobox(fgi->m_cbOperation, cbOperation, lengthof(cbOperation), -1, DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
		dialog->add_item(dialog::create_separator());
	//	dialog->add_item(dialog::create_label(txComparation));
	//	dialog->add_item(dialog::create_combobox(reinterpret_cast<ssize_t*>(&fgi->cbValue_Comparation), cbComparation, Base::lengthof(cbComparation), DIF_DROPDOWNLIST | DIF_LISTNOAMPERSAND));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbMask, cbMask, 16));
		dialog->add_item_after(dialog::create_edit(fgi->m_edMask, 20));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbDoRecursive, cbDoRecursive, 16));
		dialog->add_item(dialog::create_separator(txFileFilters, DIF_CENTERTEXT));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileReadOnly, cbFilterFileReadOnly, 24));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirReadOnly, cbFilterDirReadOnly, 24));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileHidden, cbFilterFileHidden, 24));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirHidden, cbFilterDirHidden, 24));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileSystem, cbFilterFileSystem, 24));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirSystem, cbFilterDirSystem, 24));
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileLink, cbFilterFileLink, 24));
		dialog->add_item_after(dialog::create_checkbox(fgi->m_cbFilterDirLink, cbFilterDirLink, 24));
		dialog->add_item(dialog::create_separator());
		dialog->add_item(dialog::create_checkbox(fgi->m_cbFilterFileSize, cbFilterFileSize, -1));
	//	dialog->add_item(dialog::create_separator(txFileRestrictions, DIF_CENTERTEXT));
	//	dialog->add_item(dialog::create_checkbox(fgi->m_cbRestrictionFileTime, cbRestrictionFileTime));
	//	dialog->add_item(dialog::create_checkbox(fgi->m_cbRestrictionFileAttributes, cbRestrictionFileAttributes));
	//	dialog->add_item(dialog::create_separator());
	//	dialog->add_item(dialog::create_label(txWhitespace));
	//	dialog->add_item_after(dialog::create_edit(fgi->edValue_Whitespaces, 10));
	//	dialog->add_item(dialog::create_separator());
	//	dialog->add_item(dialog::create_checkbox(fgi->cbValue_Selected, cbSelected, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
	//	dialog->add_item(dialog::create_checkbox(fgi->cbValue_AsEmpty, cbAsEmpty, (fgi->get_block_type() != BTYPE_COLUMN) ? DIF_DISABLE : 0));
		dialog->add_item(dialog::create_separator());
		dialog->add_OKCancel(message::get(message::txtBtnOk), message::get(message::txtBtnCancel));
		LogTraceObjLn();
		if (dialog->show() == 0) {
			fgi->save_settings();

			if (info->OpenFrom == OPEN_PLUGINSMENU || info->OpenFrom == OPEN_FROMMACRO) {
				auto apanel = far3::open_panel(true);
//				auto ppanel = far3::open_panel(false);
				if (apanel) {
					auto api = apanel->get_info();
					auto curDir = apanel->get_current_directory();
					LogTrace(L"pi->curr_dir: '%s'\n", curDir);
					LogTrace(L"pi->StructSize: %Iu\n", api->StructSize);
					LogTrace(L"pi->PluginHandle: %p\n", api->PluginHandle);
					LogTrace(L"pi->Flags: %I64X\n", api->Flags);
					LogTrace(L"pi->ItemsNumber: %Iu\n", api->ItemsNumber);
					LogTrace(L"pi->SelectedItemsNumber: %Iu\n", api->SelectedItemsNumber);
					LogTrace(L"pi->CurrentItem: %Iu\n", api->CurrentItem);
					LogTrace(L"pi->TopPanelItem: %Iu\n", api->TopPanelItem);
					auto appi = apanel->get_current();
					auto afileName = appi->FileName;
					LogTrace(L"appi->FileName: '%s'\n", afileName);
//					if (cstr::find(fileName, PATH_SEPARATORS)) {
//						cstr::copy(buf2, fileName, lengthof(buf2));
//					} else {
//						cstr::copy(buf2, panel->get_current_directory(), lengthof(buf2));
//						if (!cstr::is_empty(buf2)) {
//							Far::fsf().AddEndSlash(buf2);
//						}
//						cstr::cat(buf2, fileName, lengthof(buf2));
//					}

//					auto pppi = ppanel->get_current();
//					auto pfileName = pppi->FileName;
//					auto ppi = ppanel->get_info();
//					LogTrace(L"pi->StructSize: '%Iu'\n", ppi->StructSize);
//					LogTrace(L"pi->PluginHandle: '%p'\n", ppi->PluginHandle);
//					LogTrace(L"pi->Flags: '%I64X'\n", ppi->Flags);
//					LogTrace(L"pi->ItemsNumber: '%Iu'\n", ppi->ItemsNumber);
//					LogTrace(L"pi->SelectedItemsNumber: '%Iu'\n", ppi->SelectedItemsNumber);
//					LogTrace(L"pi->CurrentItem: '%Iu'\n", ppi->CurrentItem);
//					LogTrace(L"pi->TopPanelItem: '%Iu'\n", ppi->TopPanelItem);
//
//					LogTrace(L"pi->curr_dir: '%s'\n", ppanel->get_current_directory());
//					LogTrace(L"pi->FileName: '%s'\n", pfileName);
//					LogTrace(L"ppi->FileName: '%s'\n", pfileName);

					auto acurrent_dir = simstd::make_shared<fsys::Folder>(curDir);
					apanel->start_selection();
					for (size_t i = apanel->selected(); i; --i) {
						auto item = apanel->get_selected(0);
						if (item) {
							if (fsys::is_dir(item->FileAttributes))
								global::vars().folders.emplace_back(simstd::make_shared<fsys::Folder>(item->FileName, acurrent_dir));
							else
								global::vars().files.emplace_back(simstd::make_shared<fsys::File>(item->FileName, acurrent_dir));
						}
						apanel->unselect(0);
					}
					apanel->commit_selection();
				}
			}

			FileProcessor().execute();
		}
	}
	LogTraceObjLn();

	return nullptr;
}

///=================================================================================================
far3::Plugin_i* create_FarPlugin(const PluginStartupInfo* psi)
{
	return new FarPlugin(psi);
}
