#include <global.hpp>

#include <basis/sys/logger.hpp>

namespace global {

	Statistics::Statistics()
	{
		LogTraceObj();
		memset(this, 0, sizeof(*this));
	}

	Options::Options()
	{
		LogTraceObj();
		firstBlockHash = 65 * 1024;
		showStatistics = 1;
		attrMustMatch = 0;
		timeMustMatch = 0;
		doHardlink = 0;
		{
			auto& bunch = searchOptions.add_filter_bunch(fsys::Sequence::FiltersBunch::Type::ExcludeAll, L"exclude readonly archive files");
			bunch.add_filter(fsys::Sequence::Filter::ByAttr(FILE_ATTRIBUTE_DIRECTORY, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_ARCHIVE));
		}
		{
			auto& bunch = searchOptions.add_filter_bunch(fsys::Sequence::FiltersBunch::Type::ExcludeAll, L"exclude zero sized files");
			bunch.add_filter(fsys::Sequence::Filter::BySize(0, 0));
			bunch.add_filter(fsys::Sequence::Filter::ByAttr(FILE_ATTRIBUTE_DIRECTORY));
		}
		{
			auto& bunch = searchOptions.add_filter_bunch(fsys::Sequence::FiltersBunch::Type::ExcludeAll, L"exclude certain sized files");
			bunch.add_filter(fsys::Sequence::Filter::BySize(500 * 1024));
			bunch.add_filter(fsys::Sequence::Filter::ByAttr(FILE_ATTRIBUTE_DIRECTORY));
		}
//		{
//			auto& bunch = searchOptions.add_filter_bunch(fsys::Sequence::FiltersBunch::Type::ExcludeAll, L"exclude dirs");
//			bunch.add_filter(fsys::Sequence::Filter::ByAttr(FILE_ATTRIBUTE_DIRECTORY, 0));
//		}
//		{
//			auto& bunch = searchOptions.add_filter_bunch(fsys::Sequence::FiltersBunch::Type::IncludeOnly, L"include qwe1");
//			bunch.add_filter(fsys::Sequence::Filter::BySize(1024, 1 * 1024 * 1024));
////			auto enabledAttr = ~(FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_REPARSE_POINT);
////			auto enabledAttr = ~static_cast<uint32_t>(0);
//			bunch.add_filter(fsys::Sequence::Filter::ByAttr(0, FILE_ATTRIBUTE_READONLY));
////			filter.set_attr(FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_SYSTEM);
//		}
	}

	Statistics & statistics()
	{
		static Statistics instance;
		return instance;
	}

	Options & options()
	{
		static Options instance;
		return instance;
	}

	Vars & vars()
	{
		static Vars instance;
		return instance;
	}

}
