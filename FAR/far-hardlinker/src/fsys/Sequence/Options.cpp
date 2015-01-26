#include <fsys.hpp>

#include <basis/sys/cstr.hpp>

fsys::Sequence::Options::Options()
{
}

fsys::Sequence::FiltersBunch& fsys::Sequence::Options::add_filter_bunch(FiltersBunch::Type type, const ustring& name)
{
	filters.emplace_back(type, name);
	return filters.back();
}

bool fsys::Sequence::Options::apply_filters(const FindStat& stat) const
{
	bool skip = false;

	LogConsoleDebug(-1, L"  %s: 0x%08X, %I64u, '%s'\n", L"found", stat.attr(), stat.size(), stat.name());
	if (cstr::compare(stat.name(), L".") == 0 || cstr::compare(stat.name(), L"..") == 0) {
//		LogConsoleDebug(FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN, L"  ignored [invalid]: '%s'\n", stat.name());
		skip = true;
	} else {
		for (const Sequence::FiltersBunch& filter : filters)
		{
			if (filter(stat, statistics)) {
				skip = true;
				break;
			}
		}
	}

	LogConsoleDebug(-1, L"  %s: 0x%08X, %I64u, '%s'\n", skip ? L"skipped" : L"accepted", stat.attr(), stat.size(), stat.name());
	return skip;
}
