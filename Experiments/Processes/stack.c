#include <stdio.h>

int main() {

	foo:

		printf("the code: %p\n", &&foo);

	for(;;){}
	return 0;
}
