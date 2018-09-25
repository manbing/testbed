#include <stdio.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdlib.h>
#include <unistd.h>

#define PTHREAD_NUM 3

void *pthread_work(void *parameter)
{
    unsigned long tid;

    tid = syscall(SYS_gettid);

    printf("tid = %lu, IN...\n", tid);
    printf("tid %lu, pthread_self return %p\n", tid, (void *)pthread_self());
    printf("tid = %lu, EXIT...\n", tid);

    return NULL;
}

int main (void)
{
    pthread_t thread [PTHREAD_NUM];
    int i = 0;

    for (i = 0; i < PTHREAD_NUM; i++) {
        pthread_create(&thread[i], NULL, pthread_work, NULL);
        sleep(3);
    }

#ifdef JOIN
    printf("Join thread....\n");
    for (i = 0; i < PTHREAD_NUM; i++) {
        pthread_join(thread[i], NULL);
    }
#endif
    
    printf("Start sleep....\n");
    sleep(60);
    printf("Wake up....\n");

    for (i = 0; i < PTHREAD_NUM; i++) {
        pthread_create(&thread[i], NULL, pthread_work, NULL);
        sleep(3);
    }

    pause();
    
    return 0;
}
