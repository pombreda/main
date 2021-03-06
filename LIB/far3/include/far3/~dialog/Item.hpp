﻿/**
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

#ifndef _FAR3_DIALOG_ITEM_HPP_
#define _FAR3_DIALOG_ITEM_HPP_

namespace far3
{
	namespace dialog
	{
		struct Item:
			public FarDialogItem,
			private pattern::Uncopyable
		{
			~Item();

			Item(ssize_t min_width, FARDIALOGITEMTYPES type, const wchar_t* text, FARDIALOGITEMFLAGS flags = DIF_NONE);

			Item(ItemBinding* binding, FARDIALOGITEMTYPES type, const wchar_t* text, FARDIALOGITEMFLAGS flags = DIF_NONE);

			Item(Item && other);

			Item & operator =(Item && other);

			ssize_t get_height() const;

			ssize_t get_width() const;

			ssize_t get_text_width() const;

			void set_dialog(HANDLE dialog);

			void set_index(ssize_t index);

			void set_dimensions(ssize_t x, ssize_t y, ssize_t width, ssize_t height = 1);

			void save() const;
		};

		Item create_label(const wchar_t* text, ssize_t min_width, FARDIALOGITEMFLAGS flags = DIF_NONE);

		inline Item create_label(ssize_t msg_id, ssize_t min_width, FARDIALOGITEMFLAGS flags = DIF_NONE)
		{
			return create_label(message::get(msg_id), min_width, flags);
		}

		Item create_separator(const wchar_t* text = nullptr, FARDIALOGITEMFLAGS flags = DIF_NONE);

		inline Item create_separator(ssize_t msg_id, FARDIALOGITEMFLAGS flags = DIF_NONE)
		{
			return create_separator(message::get(msg_id), flags);
		}

		Item create_checkbox(ssize_t& value, const wchar_t* text, ssize_t min_width, FARDIALOGITEMFLAGS flags = DIF_NONE);

		inline Item create_checkbox(ssize_t& value, ssize_t msg_id, ssize_t min_width, FARDIALOGITEMFLAGS flags = DIF_NONE)
		{
			return create_checkbox(value, message::get(msg_id), min_width, flags);
		}

		Item create_combobox(ssize_t& value, FarListItem items[], size_t count, ssize_t min_width, FARDIALOGITEMFLAGS flags = DIF_NONE);

		Item create_edit(simstd::wstring& value, ssize_t min_width = -1, const wchar_t* history_id = nullptr, bool use_last_history = false, FARDIALOGITEMFLAGS flags = DIF_NONE);

		Item create_password(simstd::wstring& value, ssize_t min_width = -1, FARDIALOGITEMFLAGS flags = DIF_NONE);

		struct AddRadioButton_t {
			ssize_t id;
			FARDIALOGITEMFLAGS flags;
		};
	}
}

#endif
