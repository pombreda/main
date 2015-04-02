#include <basis/tst.hpp>
#include <basis/sys/logger.hpp>

namespace tst
{
	ssize_t A::objects = 0;

	A::~A()
	{
		LogTraceObjLn();
		LogTrace2(L"val: %d\n", m_a);
		--objects;
	}

	A::A() :
		j(),
		i(),
		m_a()
	{
		LogTraceObjLn();
		++objects;
	}

	A::A(int a) :
		j(),
		i(),
		m_a(a)
	{
		LogTraceObjLn();
		LogTrace2(L"%d\n", m_a);
		++objects;
	}

	A::A(const A& other) :
		j(),
		i(),
		m_a(other.m_a)
	{
		LogTraceObjLn();
		++objects;
	}

	A::A(A&& other) :
		j(),
		i(),
		m_a(simstd::move(other.m_a))
	{
		LogTraceObjLn();
		++objects;
	}

	A& A::operator =(const A& other)
	{
		LogTraceObjLn();
		m_a = other.m_a;
		return *this;
	}

	A& A::operator =(A&& other)
	{
		LogTraceObjLn();
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
		LogTraceObjLn();
		using simstd::swap;
		swap(m_a, other.m_a);
	}

	int A::val() const
	{
		return m_a;
	}

}
