/**
	filever: File Version FAR plugin
	Displays version information from file resource in dialog
	FAR3 plugin

	c 2013 Andrew Grechkin

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

#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "18";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.10";

	//Software Status
	static const char STATUS[] = "Release";
	static const char STATUS_SHORT[] = "r";

	//Standard Version Type
	static const long MAJOR = 1;
	static const long MINOR = 1;
	static const long BUILD = 21;
	static const long REVISION = 0;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 21;
	#define RC_FILEVERSION 1,1,21,0
	#define RC_FILEVERSION_STRING "1, 1, 21, 0\0"
	static const char FULLVERSION_STRING[] = "1.1.21.0";

}

#endif
