#include <stdio.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

void pt_action(void)
{
	for (;;) {
		sleep(10);
		printf("this slave thread\n");
	}
}

void sig_handler(int num)
{
	printf("this signal handler\n");
	for (;;) {
		sleep(10);
		printf("this signal handler\n");
	}
}

int main(void)
{
	pthread_t id;
	int ret;

	signal(SIGINT, sig_handler);

	ret = pthread_create(&id, NULL, (void *)pt_action, NULL);

	for (;;) {
		sleep(10);
		printf("this main thread\n");
	}

	return 0;
}
