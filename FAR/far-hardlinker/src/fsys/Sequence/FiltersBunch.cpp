#include <fsys.hpp>

#include <basis/simstd/algorithm>

namespace {
	inline const wchar_t* matched_or_not(bool passed)
	{
		return passed ? L"    matched" : L"not matched";
	}
}

const wchar_t* to_str(fsys::Sequence::FiltersBunch::Type type)
{
	auto ret = L"I";
	switch (type) {
		case fsys::Sequence::FiltersBunch::Type::IncludeOnly:
			break;
		case fsys::Sequence::FiltersBunch::Type::ExcludeAll:
			ret = L"E";
			break;
	}
	return ret;
}

fsys::Sequence::Filter::ByAttr::ByAttr(Attr exclude, Attr include)
	: exclude(exclude)
	, include(include)
{
}

bool fsys::Sequence::Filter::ByAttr::operator ()(const Stat_i& stat) const
{
	bool passed = (stat.attr() & exclude) == Attr() && (include == ~Attr() || (stat.attr() & include) == include);

	LogConsoleDebug2(-1, L"    %s [attr: 0x%08X %% (!0x%08X && 0x%08X)]\n", matched_or_not(passed), stat.attr(), exclude, include);

	return passed;
}

void fsys::Sequence::Filter::ByAttr::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByAttr::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::BySize::BySize(fsys::Size from, fsys::Size to):
	minSize(from),
	maxSize(to)
{
}

bool fsys::Sequence::Filter::BySize::operator ()(const fsys::Stat_i& stat) const
{
	bool passed = simstd::between(minSize, stat.size(), maxSize);

	LogConsoleDebug2(-1, L"    %s [size: %I64u %% (%I64u, %I64u)]\n", matched_or_not(passed), stat.size(), minSize, maxSize);

	return passed;
}

void fsys::Sequence::Filter::BySize::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::BySize::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByMask::ByMask(const ustring& mask):
	mask(mask)
{
}

bool fsys::Sequence::Filter::ByMask::operator ()(const fsys::Stat_i& /*stat*/) const
{//FIXME
	bool passed = true;

	LogConsoleDebug2(-1, L"    %s [mask(%s)]\n", matched_or_not(passed), mask.c_str());

	return passed;
}

void fsys::Sequence::Filter::ByMask::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByMask::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByWrTime::ByWrTime(Time from, Time to):
	minTime(from),
	maxTime(to)
{
}

bool fsys::Sequence::Filter::ByWrTime::operator ()(const Stat_i& stat) const
{
	bool passed = simstd::between(minTime, stat.mtime(), maxTime);

	LogConsoleDebug2(-1, L"    %s [wrtime: %I64d %% (%I64d, %I64d)]\n", matched_or_not(passed), stat.mtime(), minTime, maxTime);

	return passed;
}

void fsys::Sequence::Filter::ByWrTime::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByWrTime::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByCrTime::ByCrTime(Time from, Time to):
	minTime(from),
	maxTime(to)
{
}

bool fsys::Sequence::Filter::ByCrTime::operator ()(const Stat_i& stat) const
{
	bool passed = simstd::between(minTime, stat.ctime(), maxTime);

	LogConsoleDebug2(-1, L"    %s [crtime: %I64d %% (%I64d, %I64d)]\n", matched_or_not(passed), stat.ctime(), minTime, maxTime);

	return passed;
}

void fsys::Sequence::Filter::ByCrTime::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByCrTime::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::Filter::ByAcTime::ByAcTime(Time from, Time to):
	minTime(from),
	maxTime(to)
{
}

bool fsys::Sequence::Filter::ByAcTime::operator ()(const Stat_i& stat) const
{
	bool passed = simstd::between(minTime, stat.atime(), maxTime);

	LogConsoleDebug2(-1, L"    %s [actime: %I64d %% (%I64d, %I64d)]\n", matched_or_not(passed), stat.atime(), minTime, maxTime);

	return passed;
}

void fsys::Sequence::Filter::ByAcTime::destroy()
{
	delete this;
}

fsys::Sequence::Filter* fsys::Sequence::Filter::ByAcTime::clone() const
{
	return new this_type(*this);
}

fsys::Sequence::FiltersBunch::~FiltersBunch()
{
	LogTraceObj();
}

fsys::Sequence::FiltersBunch::FiltersBunch(Type type, const ustring& name):
	name(name),
	type(type)
{
	LogTraceObj();
	for (auto it = bunch.cbegin(); it != bunch.cend(); ++it)
		(*it)->destroy();
}

bool fsys::Sequence::FiltersBunch::operator ()(const Stat_i& stat, Statistics& /*statistics*/) const
{ // return true if skip this item
	LogConsoleDebug2(-1, L"   appply filter [%s, '%s'] on '%s'\n", to_str(type), name.c_str(), stat.name());

	bool matched = true;
	for (auto it = bunch.cbegin(); it != bunch.cend() && matched; ++it) {
		matched = (*it)->operator()(stat);
	}

	return (type == Type::IncludeOnly && !matched) || (type == Type::ExcludeAll && matched);
}

fsys::Sequence::FiltersBunch::Type fsys::Sequence::FiltersBunch::get_type() const
{
	return type;
}

void fsys::Sequence::FiltersBunch::add_filter(const Filter& filter)
{
	bunch.emplace_back(filter.clone());
}
