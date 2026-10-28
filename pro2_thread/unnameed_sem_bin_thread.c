#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <pthread.h>

int shard_num = 0;
sem_t unnamed_sem;

void *plusOne(void *argv) {
    //信号量互斥
    sem_wait(&unnamed_sem);
    int tmp = shard_num + 1;
    shard_num = tmp;
    //信号量唤醒
    sem_post(&unnamed_sem);
}

int main() {
    //提前初始化信号量
    /**
     * int sem_init (sem_t *__sem, int __pshared, unsigned int __value)
     * sem_t *__sem :填写信号量地址
     * int __pshared：使用范围，0表示线程间共享，非0表示进程间共享
     * unsigned int __value：信号量初始值
     * return：成功返回0，失败返回-1
     * */

    int sem_r = sem_init(&unnamed_sem, 0, 1);
    if (sem_r != 0)
    {
        perror("sem_init error");
        return -1;
    }
    

    pthread_t tid[10000];
    for (int i = 0; i < 10000; i++) {
        pthread_create(tid + i, NULL, plusOne, NULL);
    }

    for (int i = 0; i < 10000; i++) {
        pthread_join(tid[i], NULL);
    }
    
    printf("shard_num is %d\n", shard_num);

    //销毁信号量
    int des_r = sem_destroy(&unnamed_sem);
    if (des_r != 0)
    {
        perror("sem_destroy error");
        return -1;
    }
    return 0;
}