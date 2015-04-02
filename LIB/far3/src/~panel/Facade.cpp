/**
 © 2014 Andrew Grechkin
 Source code: <http://code.google.com/p/andrew-grechkin>

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program. If not, see <http://www.gnu.org/licenses/>.
 **/

#include <far3/panel.hpp>

#include <basis/sys/logger.hpp>

namespace far3
{
	namespace panel
	{
		using heap_type = memory::heap::DefaultStat;

		struct FacadeImpl:
			public Facade
		{
			~FacadeImpl();

			FacadeImpl(bool active);

			bool is_valid() const;

			const PanelInfo* get_info() const override;

			bool is_active() const override;

			size_t size() const override;

			size_t selected() const override;

			size_t current() const override;

			PANELINFOFLAGS get_flags() const override;

			const wchar_t* get_current_directory() const override;

			const PluginPanelItem* operator [](size_t index) const override;

			const PluginPanelItem* get_selected(size_t index) const override;

			const PluginPanelItem* get_current() const override;

			void start_selection() override;

			void select(size_t index, bool in) override;

			void unselect(size_t index) override;

			void commit_selection() override;

		private:
			const HANDLE               m_hndl;
			PanelInfo                  m_pi;
			mutable PluginPanelItem*   m_ppi;
			mutable FarPanelDirectory* m_dir;
		};

		FacadeImpl::~FacadeImpl()
		{
			LogTraceObjLn();

			HostFree(heap_type, m_dir);
			HostFree(heap_type, m_ppi);
		}

		FacadeImpl::FacadeImpl(bool activePanel)
			: m_hndl(activePanel ? PANEL_ACTIVE : PANEL_PASSIVE)
			, m_ppi(nullptr)
			, m_dir(nullptr)
		{
			LogTraceObj(L"begin\n");
			LogTrace(L"[%d, %p]\n", activePanel, m_hndl);
			memory::zero(m_pi);
			m_pi.StructSize = sizeof(m_pi);

			m_pi.StructSize = psi().PanelControl(m_hndl, FCTL_GETPANELINFO, 0, &m_pi);
			LogTraceObj(L"end\n");
		}

		inline bool FacadeImpl::is_valid() const
		{
			return m_pi.StructSize;
		}

		const PanelInfo* FacadeImpl::get_info() const
		{
			return &m_pi;
		}

		bool FacadeImpl::is_active() const
		{
			return m_hndl == PANEL_ACTIVE;
		}

		size_t FacadeImpl::size() const
		{
			return m_pi.ItemsNumber;
		}

		size_t FacadeImpl::selected() const
		{
			return m_pi.SelectedItemsNumber;
		}

		size_t FacadeImpl::current() const
		{
			return m_pi.CurrentItem;
		}

		PANELINFOFLAGS FacadeImpl::get_flags() const
		{
			return m_pi.Flags;
		}

		const wchar_t* FacadeImpl::get_current_directory() const
		{
			const wchar_t* ret = L"";
			size_t size = psi().PanelControl(m_hndl, FCTL_GETPANELDIRECTORY, 0, nullptr);
			m_dir = static_cast<decltype(m_dir)>(HostRealloc(heap_type, m_dir, size));
			m_dir->StructSize = sizeof(*m_dir);
			if (psi().PanelControl(m_hndl, FCTL_GETPANELDIRECTORY, size, m_dir) && m_dir->Name) {
				ret = m_dir->Name;
			}
			LogTrace(L"-> '%s'\n", ret);
			return ret;
		}

		const PluginPanelItem* FacadeImpl::operator [](size_t index) const
		{
			size_t m_ppiSize = psi().PanelControl(m_hndl, FCTL_GETPANELITEM, index, nullptr);
			LogTrace2(L"size: %Iu\n", m_ppiSize);
			m_ppi = static_cast<decltype(m_ppi)>(HostRealloc(heap_type, m_ppi, m_ppiSize));

			FarGetPluginPanelItem gpi = {sizeof(gpi), m_ppiSize, m_ppi};
			psi().PanelControl(m_hndl, FCTL_GETPANELITEM, index, &gpi);

			LogTrace(L"[%Iu] -> %p\n", index, m_ppi);
			return m_ppi;
		}

		const PluginPanelItem* FacadeImpl::get_selected(size_t index) const
		{
			size_t m_ppiSize = psi().PanelControl(m_hndl, FCTL_GETSELECTEDPANELITEM, index, nullptr);
			m_ppi = static_cast<decltype(m_ppi)>(HostRealloc(heap_type, m_ppi, m_ppiSize));

			FarGetPluginPanelItem gpi = {sizeof(gpi), m_ppiSize, m_ppi};
			psi().PanelControl(m_hndl, FCTL_GETSELECTEDPANELITEM, index, &gpi);

			LogTrace(L"[%Iu] -> %p\n", index, m_ppi);
			return m_ppi;
		}

		const PluginPanelItem* FacadeImpl::get_current() const
		{
			return operator [](m_pi.CurrentItem);
		}

		void FacadeImpl::start_selection()
		{
			LogTraceObjLn();
			psi().PanelControl(m_hndl, FCTL_BEGINSELECTION, 0, nullptr);
		}

		void FacadeImpl::select(size_t index, bool in)
		{
			LogTraceObjLn();
			psi().PanelControl(m_hndl, FCTL_SETSELECTION, index, (PVOID)in);
		}

		void FacadeImpl::unselect(size_t index)
		{
			LogTraceObjLn();
			psi().PanelControl(m_hndl, FCTL_CLEARSELECTION, index, nullptr);
		}

		void FacadeImpl::commit_selection()
		{
			LogTraceObjLn();
			psi().PanelControl(m_hndl, FCTL_ENDSELECTION, 0, nullptr);
		}

		Panel open(bool activePanel)
		{
			auto tmp = simstd::make_unique<FacadeImpl>(activePanel);
			return tmp->is_valid() ? Panel(simstd::move(tmp)) : Panel();
		}
	}
}
