#ifndef BASIS_TST_A_HPP_
#define BASIS_TST_A_HPP_

namespace tst
{
	class A
	{
	public:
		~A();

		A();
		/*explicit*/ A(int a);
		A(const A& other);
		A(A&& other);

		A& operator =(const A& other);
		A& operator =(A&& other);

		A& operator +=(const A& other);

		A operator +(const A& other) const;

		bool operator < (const A& other) const;

		void swap(A& other);

		int val() const;

		int j;
		int i;

		static ssize_t objects;

	private:
		int m_a;
	};
}

#endif
