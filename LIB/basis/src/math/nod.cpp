#include <basis/math.hpp>

ssize_t math::nod(ssize_t a, ssize_t b)
{
	while (a && b) {
		if (a < b)
			b %= a;
		else
			a %= b;
	}
	return (a + b);
}
