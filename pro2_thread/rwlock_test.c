#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_rwlock_t rwlock;
int shared_data = 0;

void *lock_reader(void *argv)
{
    //多个线程同时读共享数据 共享数据不会被修改 所以可以同时读
    pthread_rwlock_rdlock(&rwlock);
    printf("this is %s, value is %d\n", (char *)argv, shared_data);

    pthread_rwlock_unlock(&rwlock);
}

void *lock_writer(void *argv)
{
    //给多个线程添加写锁 共享数据会被修改 所以同一时间只能有一个线程写
    pthread_rwlock_wrlock(&rwlock);

    int tmp = shared_data + 1;
    sleep(1);
    shared_data = tmp;
    printf("this is %s, shared_data为%d\n", (char *)argv, shared_data);

    //释放写锁
    pthread_rwlock_unlock(&rwlock);
}

int main()
{
    //显示初始化读写锁
    pthread_rwlock_init(&rwlock, NULL);

    pthread_t writer1, writer2, reader1, reader2, reader3, reader4, reader5, reader6;
    
    
    pthread_create(&writer1, NULL, lock_writer, "writer1");
    pthread_create(&writer2, NULL, lock_writer, "writer2");
    sleep(3);
    pthread_create(&reader1, NULL, lock_reader, "reader1");
    pthread_create(&reader2, NULL, lock_reader, "reader2");
    pthread_create(&reader3, NULL, lock_reader, "reader3");
    pthread_create(&reader4, NULL, lock_reader, "reader4");
    pthread_create(&reader5, NULL, lock_reader, "reader5");
    pthread_create(&reader6, NULL, lock_reader, "reader6");

    pthread_join(writer1, NULL);
    pthread_join(writer2, NULL);
    pthread_join(reader1, NULL);
    pthread_join(reader2, NULL);
    pthread_join(reader3, NULL);
    pthread_join(reader4, NULL);
    pthread_join(reader5, NULL);
    pthread_join(reader6, NULL);

    //销毁读写锁
    pthread_rwlock_destroy(&rwlock);

    return 0;
}