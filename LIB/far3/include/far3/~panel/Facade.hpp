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

#ifndef _FAR3_PANEL_FACADE_HPP_
#define _FAR3_PANEL_FACADE_HPP_

#include <far3/plugin.hpp>

namespace far3 {
	namespace panel {

		struct Facade {
			virtual ~Facade() = default;

//			virtual bool is_active() const = 0;
//
//			virtual size_t size() const = 0;
//
//			virtual size_t selected() const = 0;
//
//			virtual size_t current() const = 0;
//
//			virtual PANELINFOFLAGS get_flags() const = 0;
//
			virtual const PluginPanelItem* operator [](size_t index) const = 0;
//
//			virtual const PluginPanelItem* get_selected(size_t index) const;
//
			virtual const PluginPanelItem* get_current() const = 0;
//
//			virtual void start_selection() = 0;
//
//			virtual void select(size_t index, bool in) = 0;
//
//			virtual void unselect(size_t index) = 0;
//
//			virtual void commit_selection() = 0;
		};

	}

	using Panel = simstd::unique_ptr<panel::Facade>;

	Panel open_panel(bool activePanel = true);

}

#endif
