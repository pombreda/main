#include <basis/sys/console.hpp>
#include <basis/sys/logger.hpp>
#include <basis/simstd/algorithm>
#include <basis/simstd/iterator>
#include <basis/simstd/vector>
#include <basis/tst.hpp>

#include <ctime>
#include <cassert>
#include <algorithm>

using vector_type = simstd::vector<tst::A>;
using Value = simstd::shared_ptr<ssize_t>;
using vec_t = simstd::vector<Value>;
//using vec_t = simstd::movable_vector<Value>;

bool Less(const Value& a, const Value& b)
{
	//	printFunc("%s\n", __PRETTY_FUNCTION__);
	return *a < *b;
}

//namespace {
//	class BaseCl {
//	public:
//		~BaseCl()
//		{
//			printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl() :
//			m_val()
//		{
//			printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl(int val) :
//			m_val(val)
//		{
//			printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl(const BaseCl & other) :
//			m_val(other.m_val)
//		{
//			//printFunc("%s\n", __PRETTY_FUNCTION__);
//		}
//
//		BaseCl & operator =(const BaseCl & other)
//		{
//			m_val = other.m_val;
//			//printFunc("%s\n", __PRETTY_FUNCTION__);
//			return *this;
//		}
//
//		simstd::ssize_t value() const
//		{
//			return m_val;
//		}
//
//		bool operator <(const BaseCl & right) const
//		{
//			return m_val < right.m_val;
//		}
//
//	private:
//		simstd::ssize_t m_val;
//	};
//
//	//bool operator < (const Base & left, const Base & right) {return left.operator <(right);}
//
//	class Derived: public BaseCl {
//	public:
//		Derived()
//		{
//		}
//		Derived(int val) :
//			BaseCl(val)
//		{
//		}
//	};
//}

namespace hh {

	inline ssize_t get_random(ssize_t l = 100)
	{
//		printFunc("%s\n", __PRETTY_FUNCTION__);
		return rand() % l;
	}

	inline simstd::shared_ptr<ssize_t> random0p()
	{
//		printFunc("%s\n", __PRETTY_FUNCTION__);
		return simstd::shared_ptr<ssize_t>(new ssize_t(rand() % 100));
	}

}

struct MyAllocator: public simstd::allocator<size_t>
{
};

void print_container(const char* name, const vector_type& c, tst::aPrintFunc printFunc)
{
	using namespace simstd;
	printFunc("%s: capa(): %Id, size(): %Id (", name, c.capacity(), c.size());
	for (auto it = begin(c); it != end(c); ++it) {
		printFunc(" %Id", it->val());
	}
	printFunc(")\n");
}

void print_container(const char* name, const vec_t& c, tst::aPrintFunc printFunc)
{
	using namespace simstd;
	printFunc("%s: capa(): %Id, size(): %Id (", name, c.capacity(), c.size());
	for (auto it = begin(c); it != end(c); ++it) {
		printFunc(" %Id", *it->get());
	}
	printFunc(")\n");
}

ssize_t tst::_vector(tst::aPrintFunc printFunc)
{
	using namespace simstd;
	srand(static_cast<unsigned>(time(0)));


//	std::next_permutation()
//	{
////		auto_ptr<Base> a(new Base(5));
//		Base * b = new Base;
//		printFunc("%s: b: %p\n", __PRETTY_FUNCTION__, b);
////		shared_ptr<Base> s1(b);
//		shared_ptr<Base> s1(b, simstd::default_delete<Base[]>());
//		shared_ptr<Base> s2(s1);
//		printFunc("%s: get(): %p\n", __PRETTY_FUNCTION__, s1.get());
//		printFunc("%s: count(): %Iu\n", __PRETTY_FUNCTION__, s1.use_count());
//	}
//	printFunc("%s end\n", __PRETTY_FUNCTION__);
//	return 0;

	tst::A data[] {44, 45, 46, 47, 48};

	printFunc("\nconstruct v11\n");
	vector_type v11;

	printFunc("\nconstruct v12\n");
	vector_type v12 = vector_type(MyAllocator());

	printFunc("\nconstruct v21\n");
	vector_type v21(11);

	printFunc("\nconstruct v22\n");
	vector_type v22(11, MyAllocator());

	printFunc("\nconstruct v31\n");
	vector_type v31(11, tst::A(444));

	printFunc("\nconstruct v32\n");
	vector_type v32(11, tst::A(555), MyAllocator());

	printFunc("\nconstruct v41\n");
	vector_type v41(begin(data), end(data));

	printFunc("\nconstruct v42\n");
	vector_type v42(begin(data), end(data), MyAllocator());

	printFunc("\nconstruct v51\n");
	vector_type v51(v21);

	printFunc("\nconstruct v61\n");
	vector_type v61(v22, MyAllocator());

	printFunc("\nconstruct v71\n");
	vector_type v71(simstd::move(v31));

	printFunc("\nconstruct v81\n");
	vector_type v81(simstd::move(v32), MyAllocator());

	printFunc("construct end\n");

	print_container("v11", v11, printFunc);
	print_container("v12", v12, printFunc);
	print_container("v21", v21, printFunc);
	print_container("v22", v22, printFunc);
	print_container("v31", v31, printFunc);
	print_container("v32", v32, printFunc);
	print_container("v41", v41, printFunc);
	print_container("v42", v42, printFunc);
	print_container("v51", v51, printFunc);
	print_container("v61", v61, printFunc);
	print_container("v71", v71, printFunc);
	print_container("v81", v81, printFunc);

//	return 0;

//	vec_t v(10);
	vec_t v(6);
	printFunc("\ngenerate\n");
	simstd::generate(begin(v), end(v), hh::random0p);
	print_container("v", v, printFunc);

	printFunc("\nnth_element:\n");
	simstd::nth_element(begin(v), begin(v) + 1, end(v), Less);
	print_container("v", v, printFunc);

	printFunc("\nsorting back:\n");
	simstd::sort(rbegin(v), rend(v), Less);
	print_container("v", v, printFunc);

	printFunc("\nshuffling:\n");
	simstd::random_shuffle(begin(v), end(v), hh::get_random);
	print_container("v", v, printFunc);

	printFunc("\nsorting:\n");
	simstd::sort(begin(v), end(v), Less);
	print_container("v", v, printFunc);

	printFunc("\npermutating:\n");
	print_container("v", v, printFunc);
	size_t i = 0;
	while (simstd::next_permutation(begin(v), end(v), Less)) {
		printFunc("%3Iu - ", ++i);
		print_container("v", v, printFunc);
	}

//	return 0;

//	BaseCl b1(1);
//	BaseCl b2(2);
//	BaseCl b3(3);
//
//	movable_vector<BaseCl> mv1;
//	mv1.lock();
//	mv1.push_back(b1);
//	mv1.push_back(b2);
//	mv1.push_back(b3);
//	mv1.push_back(b3);
//	mv1.push_back(b3);
//	mv1.push_back(b3);
//	mv1.unlock();
//	//printFunc("mv1.capa(): %Id, mv1.size(): %Id\n", mv1.capacity(), mv1.size());
//
//	typedef vector<BaseCl> vect;
//	typedef vector<BaseCl>::iterator iter;
//	ssize_t m1[] = {0, 1, 2, 3, 4, 5};
//	//printFunc("construct v1\n");
//	vect v1;
//	//	v1.resize(5);
//	//	v1.resize(10, 9);
//	for (int i = 10; i < 30; ++i) {
//		//printFunc("v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
//		v1.push_back(i);
//	}
//	//printFunc("v1.capa(): %Id, v1.size(): %Id\n", v1.capacity(), v1.size());
//	//printFunc("construct v2\n");
//	vect v2(2, BaseCl(77));
//	v2.insert(end(v2), rbegin(v1), rend(v1));
//	//printFunc("construct v3\n");
//	vect v3(vector<int>::size_type(3), Derived(63));
//	v2.insert(begin(v2) + 10, v3.begin(), v3.end());
//	//printFunc("construct v4\n");
//	vect v4(m1, m1 + 6);
//	v4.insert(begin(v4), rbegin(m1), rend(m1));
//	//printFunc("v4.capa(): %Id, v4.size(): %Id (", v4.capacity(), v4.size());
//	for (iter it = begin(v4); it != end(v4); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	//printFunc("construct v5\n");
//	vect v5(v4);
//	//printFunc("v1.capa(): %Id, v1.size(): %Id (", v1.capacity(), v1.size());
//	for (iter it = begin(v1); it != end(v1); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	return 0;
//
//	//printFunc("before v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
//	for (iter it = begin(v2); it != end(v2); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" ) mid: %Id\n", (begin(v2) + v2.size() / 2)->value());
//	//	iter mid = simstd::_quick_sort_split(begin(v2), end(v2));
//	simstd::sort(begin(v2), end(v2));
//	//printFunc("after  v2.capa(): %Id, v2.size(): %Id (", v2.capacity(), v2.size());
//	for (iter it = begin(v2); it != end(v2); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//	//printFunc(" ) mid: %Id\n", mid->value());
//	//printFunc(" )\n");
//	//printFunc("v3.capa(): %Id, v3.size(): %Id (", v3.capacity(), v3.size());
//	for (iter it = begin(v3); it != end(v3); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	//	v1.insert(v1.end(), m1, m1 + 6);
//
//	//	v1.insert(v1.cend(), 5, 0);
//	//	simstd::vector<int> v2(5);
//	//	for (const int * it = cbegin(m1); it != cend(m1); ++it)
//	//	{
//	//		//printFunc("%Id\n", *it);
//	//	}
//	for (vect::reverse_iterator it = rbegin(v5); it != rend(v5); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc("\n");
//	//	//printFunc("v5.clear\n");
//	//	v5.clear();
//	//printFunc("v5.erase\n");
//	v5.erase(v5.begin() + 2);
//	//printFunc("v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
//	for (iter it = begin(v5); it != end(v5); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" )\n");
//	//printFunc("v5.insert\n");
//	//	iter it = v5.insert(v5.begin() + 2, (vect::size_type)1, 2);
//	//	v5.insert(v5.begin() + 2, 2);
//	//	v5.insert(v5.begin() + 3, 22);
//	//	iter it = v5.insert(v5.begin() + 4, 222);
//	//	int m2[] = {2, 22, 222};
//	//	iter it = v5.insert(v5.begin() + 2, m2, m2 + 3);
//	//printFunc("v5.capa(): %Id, v5.size(): %Id (", v5.capacity(), v5.size());
//	for (iter it = begin(v5); it != end(v5); ++it) {
//		//printFunc(" %Id", it->value());
//	}
//	//printFunc(" ) %Id\n", it->value());
	return 0;
}
