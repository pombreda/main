#ifndef BASIS_SYS_FSYS_DIRECTORY_HPP_
#define BASIS_SYS_FSYS_DIRECTORY_HPP_

namespace fsys {

	namespace directory {

		bool del_nt(const wchar_t* path);

		bool create_nt(const wchar_t* path, LPSECURITY_ATTRIBUTES lpsa = nullptr);

		bool copy_link(const wchar_t* path, const wchar_t* dest, LPSECURITY_ATTRIBUTES lpsa = nullptr);

	}

}

#endif
