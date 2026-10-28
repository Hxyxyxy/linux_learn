#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

sem_t *full;
sem_t *empty;

int shard_num;

int rand_num()
{
    srand(time(NULL));

    return rand();
}

void *producer(void *argv)
{
    for (int i = 0; i < 5; i++)
    {
        //获取信号量
        //最开始，empty信号量为1，可以-1 正常执行
        //下次到这里，empty信号量为0，不能-1，阻塞等待
        sem_wait(empty);
        printf("\n==========> 第 %d 轮数据传输 <=========\n\n", i + 1);
        sleep(1);
        shard_num = rand_num();
        printf("producer has sent data\n");
        //释放信号量
        //使full+1 此时 消费者能够获取full信号量，进行消费
        sem_post(full);
    }
}

void *consumer(void *argv)
{
    for (int i = 0; i < 5; i++)
    {
        //最开始，full信号量为0，不能-1，阻塞等待
        sem_wait(full);
        printf("consumer %d has read data\n", i + 1);
        
        sleep(1);
        printf("the shard_num is %d\n", shard_num);
        //消费者执行一轮 释放信号量，使empty+1 此时 生产者能够获取empty信号量，进行生产
        sem_post(empty);
    }
}

int main()
{
    full = malloc(sizeof(sem_t));
    empty = malloc(sizeof(sem_t));

    //初始化信号量
    sem_init(empty, 0, 1);
    sem_init(full, 0, 0);

    
    //创建生产者线程和消费者线程
    pthread_t producer_id, consumer_id;
    pthread_create(&producer_id, NULL, producer, NULL);
    pthread_create(&consumer_id, NULL, consumer, NULL);

    //等待线程全部执行完成
    pthread_join(producer_id, NULL);
    pthread_join(consumer_id, NULL);

    //摧毁信号量
    sem_destroy(empty);
    sem_destroy(full);

    return 0;
}