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

#ifndef _LIBFAR_DIALOG_BUIDER_HPP_
#define _LIBFAR_DIALOG_BUIDER_HPP_

#include <libfar3/plugin.hpp>
#include <libfar3/helper.hpp>

#include <basis/ext/pattern.hpp>

#include <basis/simstd/memory>
#include <basis/simstd/string>

namespace Far {

	struct DialogItemBinding_i {
		virtual ~DialogItemBinding_i() = default;

		HANDLE get_dlg() const;

		ssize_t get_index() const;

		void set_dlg(HANDLE * hndl);

		void set_index(ssize_t index);

		void save() const;

		ssize_t get_height() const;

		ssize_t get_width() const;

	protected:
		DialogItemBinding_i() :
			m_dlg(),
			m_index()
		{
		}

		DialogItemBinding_i(HANDLE * dlg, ssize_t index) :
			m_dlg(dlg),
			m_index(index)
		{
		}

	private:
		virtual void save_() const = 0;

		virtual ssize_t get_height_() const;

		virtual ssize_t get_width_() const = 0;

		HANDLE * m_dlg;
		ssize_t m_index;
	};

	struct FarDialogItem_t: public FarDialogItem, private pattern::Uncopyable {
		~FarDialogItem_t();

		FarDialogItem_t(FARDIALOGITEMTYPES Type_, PCWSTR Text_, FARDIALOGITEMFLAGS flags_ = DIF_NONE);

		FarDialogItem_t(DialogItemBinding_i * binding, FARDIALOGITEMTYPES Type_, PCWSTR Text_, FARDIALOGITEMFLAGS flags_ = DIF_NONE);

		FarDialogItem_t(FarDialogItem_t && right);

		FarDialogItem_t & operator = (FarDialogItem_t && right);

		ssize_t get_height() const;

		ssize_t get_width() const;

		void set_dlg(HANDLE * dlg);

		void set_index(ssize_t ind);

		void set_dimension(ssize_t x, ssize_t y, ssize_t width, ssize_t height = 1);

		void save() const;
	};

	FarDialogItem_t * create_label(PCWSTR text, FARDIALOGITEMFLAGS flags = DIF_NONE);

	inline FarDialogItem_t * create_label(ssize_t msg_id, FARDIALOGITEMFLAGS flags = DIF_NONE)
	{
		return create_label(get_msg(msg_id), flags);
	}

	FarDialogItem_t * create_separator(PCWSTR text = nullptr, FARDIALOGITEMFLAGS flags = DIF_NONE);

	inline FarDialogItem_t * create_separator(ssize_t msg_id, FARDIALOGITEMFLAGS flags = DIF_NONE)
	{
		return create_separator(get_msg(msg_id), flags);
	}

	FarDialogItem_t * create_checkbox(ssize_t * value, PCWSTR text, FARDIALOGITEMFLAGS flags = DIF_NONE);

	inline FarDialogItem_t * create_checkbox(ssize_t * value, ssize_t msg_id, FARDIALOGITEMFLAGS flags = DIF_NONE)
	{
		return create_checkbox(value, get_msg(msg_id), flags);
	}

	FarDialogItem_t * create_combobox(ssize_t * value, FarListItem items[], size_t count, FARDIALOGITEMFLAGS flags = DIF_NONE);

	FarDialogItem_t * create_edit(simstd::wstring * value, ssize_t width = -1, PCWSTR history_id = nullptr, bool use_last_history = false, FARDIALOGITEMFLAGS flags = DIF_NONE);

	FarDialogItem_t * create_password(simstd::wstring * value, ssize_t width = -1, FARDIALOGITEMFLAGS flags = DIF_NONE);

	struct AddRadioButton_t {
		ssize_t id;
		FARDIALOGITEMFLAGS flags;
	};

	///=============================================================================================
	struct DialogBuilder_i {
		virtual ~DialogBuilder_i() = default;

		DialogBuilder_i() = default;

		FarDialogItem_t * add_item(FarDialogItem_t * item);

		FarDialogItem_t * add_item_before(FarDialogItem_t * item);

		FarDialogItem_t * add_item_after(FarDialogItem_t * item);

		void add_empty_line();

		void add_OKCancel(PCWSTR OKLabel, PCWSTR CancelLabel, PCWSTR ExtraLabel = nullptr);

		void add_radiobuttons(ssize_t * Value, ssize_t OptionCount, const AddRadioButton_t list[], bool FocusOnSelected = false);

		void start_column();

		void break_column();

		void end_column();

		void start_singlebox(ssize_t Width, PCWSTR Label = EMPTY_STR, bool LeftAlign = false);

		void end_singlebox();

		int show_ex();

		bool show();

	private:
		virtual FarDialogItem_t * add_item_(FarDialogItem_t * item) = 0;

		virtual FarDialogItem_t * add_item_before_(FarDialogItem_t * item) = 0;

		virtual FarDialogItem_t * add_item_after_(FarDialogItem_t * item) = 0;

		virtual void add_empty_line_() = 0;

		virtual void add_OKCancel_(PCWSTR OKLabel, PCWSTR CancelLabel, PCWSTR ExtraLabel) = 0;

		virtual void add_radiobuttons_(ssize_t * Value, ssize_t OptionCount, const AddRadioButton_t list[], bool FocusOnSelected) = 0;

		virtual void start_column_() = 0;

		virtual void break_column_() = 0;

		virtual void end_column_() = 0;

		virtual void start_singlebox_(ssize_t Width, PCWSTR Label, bool LeftAlign) = 0;

		virtual void end_singlebox_() = 0;

		virtual int show_() = 0;
	};

	inline FarDialogItem_t * DialogBuilder_i::add_item(FarDialogItem_t * item)
	{
		return add_item_(item);
	}

	inline FarDialogItem_t * DialogBuilder_i::add_item_before(FarDialogItem_t * item)
	{
		return add_item_before_(item);
	}

	inline FarDialogItem_t * DialogBuilder_i::add_item_after(FarDialogItem_t * item)
	{
		return add_item_after_(item);
	}

	inline void DialogBuilder_i::add_empty_line()
	{
		add_empty_line_();
	}

	inline void DialogBuilder_i::add_OKCancel(PCWSTR OKLabel, PCWSTR CancelLabel, PCWSTR ExtraLabel)
	{
		add_OKCancel_(OKLabel, CancelLabel, ExtraLabel);
	}

	inline void DialogBuilder_i::add_radiobuttons(ssize_t * Value, ssize_t OptionCount, const AddRadioButton_t list[], bool FocusOnSelected)
	{
		add_radiobuttons_(Value, OptionCount, list, FocusOnSelected);
	}

	inline void DialogBuilder_i::start_column()
	{
		start_column_();
	}

	inline void DialogBuilder_i::break_column()
	{
		break_column_();
	}

	inline void DialogBuilder_i::end_column()
	{
		end_column_();
	}

	inline void DialogBuilder_i::start_singlebox(ssize_t Width, PCWSTR Label, bool LeftAlign)
	{
		start_singlebox_(Width, Label, LeftAlign);
	}

	inline void DialogBuilder_i::end_singlebox()
	{
		end_singlebox_();
	}

	inline int DialogBuilder_i::show_ex()
	{
		return show_();
	}

	inline bool DialogBuilder_i::show()
	{
		return show_() == 0;
	}

	///=============================================================================================
	simstd::shared_ptr<DialogBuilder_i> create_dialog_builder(const GUID & aId, PCWSTR TitleLabel, PCWSTR aHelpTopic = nullptr, FARWINDOWPROC aDlgProc = nullptr, void * aUserParam = nullptr);

}

#endif
