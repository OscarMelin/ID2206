#include <stdio.h>
#include <unistd.h>

int main() {

	int pid = fork();

	printf("pid = %d\n", pid);

	return 0;
}
