#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void * thread_func(void *arg)
{
    printf("线程开始执行\n");
    sleep(2);
    printf("线程执行结束\n");
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t tid;
    //创建线程
    pthread_create(&tid, NULL, (void *)thread_func, NULL);

    //使用detach标记会等待线程完成之后回收相关资源
    pthread_detach(tid);

    //主线程运行完成比子线程慢
    //主线程休眠5秒
    printf("main thread continues\n");
    
    sleep(5); //因为主线程不会在pthread_detach等待 如果父线程先结束 会导致子线程没有机会执行

    printf("主线程结束\n");
    return 0;
}