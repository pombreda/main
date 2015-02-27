#include <stdio.h>

#include <basis/simstd/memory>

int main() {
	printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);

	printf("%s:%d\n", __PRETTY_FUNCTION__, __LINE__);
	return 0;
}
