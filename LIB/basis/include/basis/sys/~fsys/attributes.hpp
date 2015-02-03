#ifndef BASIS_SYS_FSYS_ATTRIBUTES_HPP_
#define BASIS_SYS_FSYS_ATTRIBUTES_HPP_

namespace fsys {

	typedef uint64_t Size;
	typedef int64_t  Time;
	typedef uint32_t Attr;

	class Stat_i {
	public:
		virtual ~Stat_i() = default;

		virtual const wchar_t* name() const = 0;

		virtual Size size() const = 0;
		virtual Attr attr() const = 0;

		virtual Time ctime() const = 0;
		virtual Time atime() const = 0;
		virtual Time mtime() const = 0;

		bool is_file() const;
		bool is_dir() const;
		bool is_link() const;
	};

	class StatEx_i: public Stat_i {
	public:
		virtual bool operator ==(const StatEx_i& other) const;

		virtual size_t num_links() const = 0;

		virtual uint64_t volume_sn() const = 0;
		virtual uint64_t inode() const = 0;
	};

	using StatEx = simstd::unique_ptr<StatEx_i>;

	StatEx stat_ex(HANDLE hndl);
	StatEx stat_ex(const char* path);
	StatEx stat_ex(const wchar_t* path);

	Attr get_attr_nt(const wchar_t* path);
	bool set_attr_nt(const wchar_t* path, Attr attr);

	bool is_valid(Attr attr);
	bool is_file(Attr attr);
	bool is_dir(Attr attr);
	bool is_link(Attr attr);

	bool is_file_nt(const wchar_t* path);
	bool is_dir_nt(const wchar_t* path);
	bool is_link_nt(const wchar_t* path);

}

inline bool fsys::Stat_i::is_file() const
{
	return !fsys::is_dir(attr());
}
inline bool fsys::Stat_i::is_dir() const
{
	return fsys::is_dir(attr());
}
inline bool fsys::Stat_i::is_link() const
{
	return fsys::is_link(attr());
}

inline bool fsys::is_valid(fsys::Attr attr)
{
	return attr != INVALID_FILE_ATTRIBUTES;
}

inline bool fsys::is_file(fsys::Attr attr)
{
	return is_valid(attr) && !(attr & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool fsys::is_dir(fsys::Attr attr)
{
	return is_valid(attr) && (attr & FILE_ATTRIBUTE_DIRECTORY);
}

inline bool fsys::is_link(fsys::Attr attr)
{
	return is_valid(attr) && (attr & FILE_ATTRIBUTE_REPARSE_POINT);
}

#endif
