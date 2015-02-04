#ifndef _FSYS_SEQUENCE_HPP_
#define _FSYS_SEQUENCE_HPP_

#include <fsys.hpp>

#include <basis/sys/fsys.hpp>
#include <basis/simstd/string>

namespace fsys {

	class Sequence: private pattern::Uncopyable {
		typedef Sequence this_type;
		class ci_iterator;

	public:
		struct FindStat;
		class FiltersBunch;
		class Filter;
		class Options;
		class Statistics;

		typedef FindStat    value_type;
		typedef size_t      size_type;
		typedef uint64_t    flags_type;
		typedef ci_iterator iterator;
		typedef ci_iterator const_iterator;

		Sequence(const ustring& path, const ustring& mask, const Options& options);

		const_iterator begin() const;

		const_iterator end() const;

		bool empty() const;

		const ustring& path() const;

		const ustring& mask() const;

		const Options& options() const;

	private:
		ustring        m_path;
		ustring        m_mask;
		const Options& m_options;
	};

	inline const ustring& Sequence::path() const
	{
		return m_path;
	}

	inline const ustring& Sequence::mask() const
	{
		return m_mask;
	}

	///=================================================================================================================
	class Sequence::Statistics {
	public:
		Statistics();

		void init_statistics();
		void free_statistics();

		void folder_found(const FindStat& stat);
		void folder_ignored();
		void folder_ignored_archive();
		void folder_ignored_readonly();
		void folder_ignored_hidden();
		void folder_ignored_system();
		void folder_ignored_link();

		void file_found(const FindStat& stat);
		void file_ignored();
		void file_ignored_to_small();
		void file_ignored_to_big();
		void file_ignored_archive();
		void file_ignored_readonly();
		void file_ignored_hidden();
		void file_ignored_system();
		void file_ignored_link();
		void file_ignored_streamed();
		void file_ignored_compressed();
		void file_ignored_encrypted();
		void file_ignored_sparse();
		void file_ignored_temporary();
		void file_ignored_offline();
		void file_ignored_zero();

	private:
		struct Impl;
		Impl* m_impl;
	};

}

#include <fsys/Sequence/FindStat.hpp>
#include <fsys/Sequence/FiltersBunch.hpp>
#include <fsys/Sequence/Options.hpp>
#include <fsys/Sequence/Iterator.hpp>

#endif
