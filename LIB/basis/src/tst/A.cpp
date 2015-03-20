#include <basis/tst.hpp>
#include <basis/sys/logger.hpp>

namespace tst
{
	ssize_t A::objects = 0;

	A::~A()
	{
		LogTraceObj();
		LogNoise2(L"val: %d\n", m_a);
		--objects;
	}

	A::A() :
		j(),
		i(),
		m_a()
	{
		LogTraceObj();
		++objects;
	}

	A::A(int a) :
		j(),
		i(),
		m_a(a)
	{
		LogTraceObj();
		LogNoise2(L"%d\n", m_a);
		++objects;
	}

	A::A(const A& other) :
		j(),
		i(),
		m_a(other.m_a)
	{
		LogTraceObj();
		++objects;
	}

	A::A(A&& other) :
		j(),
		i(),
		m_a(simstd::move(other.m_a))
	{
		LogTraceObj();
		++objects;
	}

	A& A::operator =(const A& other)
	{
		LogTraceObj();
		m_a = other.m_a;
		return *this;
	}

	A& A::operator =(A&& other)
	{
		LogTraceObj();
		m_a = other.m_a;
		return *this;
	}

	A& A::operator +=(const A& other)
	{
		m_a += other.m_a;
		return *this;
	}

	A A::operator +(const A& other) const
	{
		A ret(*this);
		return ret += other;
	}

	bool A::operator < (const A& other) const
	{
		return m_a < other.m_a;
	}

	void A::swap(A& other)
	{
		LogTraceObj();
		using simstd::swap;
		swap(m_a, other.m_a);
	}

	int A::val() const
	{
		return m_a;
	}

}
