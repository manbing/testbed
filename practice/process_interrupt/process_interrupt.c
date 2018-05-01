#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <signal.h>
#include <pthread.h>

enum {
	SEMAPHORE,
	SPIN_LOCK,
};

static sem_t mutex;
static pthread_spinlock_t lock;
static int sync_type = SEMAPHORE;

void enter_infinite_loop()
{
	for(;;)
		sleep(3);
}

void sync_init(int sync_type)
{
	switch (sync_type) {
		case SEMAPHORE:
			sem_init(&mutex, 0, 1);
			break;
		case SPIN_LOCK:
			pthread_spin_init(&lock, 0);
			break;
	}
}

void sync_lock(int sync_type)
{
	switch (sync_type) {
		case SEMAPHORE:
			sem_wait(&mutex);
			break;
		case SPIN_LOCK:
			pthread_spin_lock(&lock);
			break;
	}
}

void sync_unlock(int sync_type)
{
	switch (sync_type) {
		case SEMAPHORE:
			sem_post(&mutex);
			break;
		case SPIN_LOCK:
			pthread_spin_unlock(&lock);
			break;
	}
}

void signal_handler(int sig)
{
	switch (sig) {
		case SIGALRM:
			printf("[%s:%d] Receive signal, %d.\n", __func__, __LINE__, sig);
			sync_lock(sync_type);
			printf("[%s:%d] interrupt enter infinite loop\n", __func__, __LINE__);
			enter_infinite_loop();
			sync_unlock(sync_type);
			break;
		default:
			printf("[%s:%d] Receive signal, %d.\n", __func__, __LINE__, sig);
			break;
	}
}

int main(void)
{
	sync_init(sync_type);
	signal(SIGALRM, signal_handler);
	sync_lock(sync_type);
	alarm(1);
	printf("[%s:%d] main enter infinite loop\n", __func__, __LINE__);
	enter_infinite_loop();
	sync_unlock(sync_type);
	printf("[%s:%d] main end\n", __func__, __LINE__);
	return 0;
}
