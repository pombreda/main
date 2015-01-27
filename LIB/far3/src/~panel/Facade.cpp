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

namespace far3 {
	namespace panel {

		struct FacadeImpl: public Facade {
			~FacadeImpl();

			FacadeImpl(bool active);

			bool is_valid() const;

//			bool is_active() const override;
//
//			size_t size() const override;
//
//			size_t selected() const override;
//
//			size_t current() const override;
//
//			PANELINFOFLAGS get_flags() const override;
//
			const PluginPanelItem* operator [](size_t index) const override;

//			const PluginPanelItem* get_selected(size_t index) const override;

			const PluginPanelItem* get_current() const override;
//
//			void start_selection() override;
//
//			void select(size_t index, bool in) override;
//
//			void unselect(size_t index) override;
//
//			void commit_selection() override;

		private:
			const HANDLE m_hndl;
			PanelInfo                  m_pi;
			mutable PluginPanelItem*   m_ppi;
//			mutable FarPanelDirectory* m_dir;
//
//			bool m_Result;
		};

		FacadeImpl::~FacadeImpl()
		{
			LogTraceObj();
		}

		FacadeImpl::FacadeImpl(bool activePanel)
			: m_hndl(activePanel ? PANEL_ACTIVE : PANEL_PASSIVE)
			, m_ppi(nullptr)
		{
			LogTraceObjBegin();
			memory::zero(m_pi);
			m_pi.StructSize = sizeof(m_pi);

			m_pi.StructSize = psi().PanelControl(m_hndl, FCTL_GETPANELINFO, 0, &m_pi);
			LogTraceObjEnd();
		}

		inline bool FacadeImpl::is_valid() const
		{
			return m_pi.StructSize;
		}

		const PluginPanelItem* FacadeImpl::operator [](size_t index) const
		{
			size_t m_ppiSize = psi().PanelControl(m_hndl, FCTL_GETPANELITEM, index, nullptr);
			LogNoise2(L"size: %Iu\n", m_ppiSize);
			m_ppi = static_cast<decltype(m_ppi)>(HostRealloc(memory::heap::DefaultStat, m_ppi, m_ppiSize));

			FarGetPluginPanelItem gpi = {sizeof(gpi), m_ppiSize, m_ppi};
			psi().PanelControl(m_hndl, FCTL_GETPANELITEM, index, &gpi);

			LogNoise(L"[%Iu] -> %p\n", index, m_ppi);
			return m_ppi;
		}

//		const PluginPanelItem* get_selected(size_t index) const
//		{
//		}

		const PluginPanelItem* FacadeImpl::get_current() const
		{
			return operator [](m_pi.CurrentItem);
		}
	}

	///=============================================================================================================
	Panel open_panel(bool activePanel)
	{
		simstd::unique_ptr<panel::FacadeImpl> tmp(new panel::FacadeImpl(activePanel));
		return tmp->is_valid() ? Panel(simstd::move(tmp)) : Panel();
	}

}
