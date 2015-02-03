#ifndef _FSYS_SEQUENCE_FINDSTAT_HPP_
#define _FSYS_SEQUENCE_FINDSTAT_HPP_

namespace fsys {

	struct Sequence::FindStat: public Stat_i {
		const wchar_t* name() const override;

		Size size() const override;
		Attr attr() const override;

		Time ctime() const override;
		Time atime() const override;
		Time mtime() const override;

	private:
		WIN32_FIND_DATAW m_stat;
		friend struct ci_iterator;
	};

}

#endif
