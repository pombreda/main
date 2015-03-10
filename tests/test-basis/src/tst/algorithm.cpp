#include <basis/simstd/algorithm>
#include <basis/tst.hpp>

namespace {
	template <typename ForwardIt>
	void print(ForwardIt first, ForwardIt last)
	{
		for (auto it = first; it != last; ++it)
		{
			TestFuncPlaceFormat("%Id ", *it);
		}
		TestFuncFormat("\n");
	}
}

ssize_t tst::_algorithm()
{
	ssize_t dim1[] = {-10, 4, 52, -22, 0, 4, 0, 0, 31, -4, 15};
	size_t size = sizeof(dim1) / sizeof(dim1[0]);

	auto first = dim1;
	auto last = dim1 + size;

	TestFuncPlaceFormat("Test sort:\n");
	TestFuncPlaceFormat("before: ");
	print(first, last);
	simstd::sort(first, last);

	TestFuncPlaceFormat("after: ");
	print(first, last);

	TestFuncPlaceFormat("Test binary search:\n");
	TestFuncPlaceFormat("[-1000]: %d\n", simstd::binary_search(first, last, -1000));
	TestFuncPlaceFormat("[+1000]: %d\n", simstd::binary_search(first, last, +1000));
	TestFuncPlaceFormat("[0]: %d\n", simstd::binary_search(first, last, 0));
	TestFuncPlaceFormat("[52]: %d\n", simstd::binary_search(first, last, 52));
	TestFuncPlaceFormat("[-22]: %d\n", simstd::binary_search(first, last, -22));
	TestFuncPlaceFormat("[53]: %d\n", simstd::binary_search(first, last, 53));
	TestFuncPlaceFormat("[-23]: %d\n", simstd::binary_search(first, last, -23));

	TestFuncPlaceFormat("Test lower bound\n");
	TestFuncPlaceFormat("[-1000]: %Id\n", simstd::lower_bound(first, last, -1000) - first);
	TestFuncPlaceFormat("[+1000]: %Id\n", simstd::lower_bound(first, last, +1000) - first);
	TestFuncPlaceFormat("[0]: %Id\n", simstd::lower_bound(first, last, 0) - first);
	TestFuncPlaceFormat("[52]: %Id\n", simstd::lower_bound(first, last, 52) - first);
	TestFuncPlaceFormat("[-22]: %Id\n", simstd::lower_bound(first, last, -22) - first);
	TestFuncPlaceFormat("[53]: %Id\n", simstd::lower_bound(first, last, 53) - first);
	TestFuncPlaceFormat("[-23]: %Id\n", simstd::lower_bound(first, last, -23) - first);

	TestFuncPlaceFormat("Test upper bound\n");
	TestFuncPlaceFormat("[-1000]: %Id\n", simstd::upper_bound(first, last, -1000) - first);
	TestFuncPlaceFormat("[+1000]: %Id\n", simstd::upper_bound(first, last, +1000) - first);
	TestFuncPlaceFormat("[0]: %Id\n", simstd::upper_bound(first, last, 0) - first);
	TestFuncPlaceFormat("[52]: %Id\n", simstd::upper_bound(first, last, 52) - first);
	TestFuncPlaceFormat("[-22]: %Id\n", simstd::upper_bound(first, last, -22) - first);
	TestFuncPlaceFormat("[53]: %Id\n", simstd::upper_bound(first, last, 53) - first);
	TestFuncPlaceFormat("[-23]: %Id\n", simstd::upper_bound(first, last, -23) - first);

	return 0;
}
