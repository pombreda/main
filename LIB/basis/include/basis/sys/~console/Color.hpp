﻿#ifndef BASIS_SYS_CONSOLE_COLOR_HPP_
#define BASIS_SYS_CONSOLE_COLOR_HPP_

namespace console {

	struct Color {
		~Color();

		Color(WORD color);

		void restore();

	private:
		bool save();

		WORD m_color;
	};

	inline Color::~Color()
	{
		restore();
	}

}

#endif
