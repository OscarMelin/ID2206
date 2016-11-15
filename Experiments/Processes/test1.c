#include<stdio.h>
#include<signal.h>
#include<unistd.h>

int volatile count;
void handler (int sig) {

	printf("signal %d ouch that hurt\n", sig);
	count++;
}

int main() {

	struct sigaction sa;
	int pid = getpid();
	printf("ok,let’s go, kill me (%d) if you can!\n", pid);
	sa.sa_handler = handler;
	sigemptyset(&sa.sa_mask);

	if(sigaction(SIGINT, &sa, NULL) != 0) {
		return(1);
	}
	while(count!=4){
	}

	printf("I've had enough!\n");
	return(0);
}
