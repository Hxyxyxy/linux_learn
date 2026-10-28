#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void * thread_func(void *arg)
{
    printf("线程开始执行\n");
    //默认取消类型是延迟
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    printf("working...\n");
    sleep(1);
    
    //人为调用取消函数
    pthread_testcancel();
    printf("线程执行结束\n");
    return NULL;
}

int main(int argc, char const *argv[])
{
    pthread_t tid;
    //创建线程
    pthread_create(&tid, NULL, (void *)thread_func, NULL);

    //取消子线程
    if (pthread_cancel(tid) != 0)
    {
        perror("pthread_cancel");
        exit(1);
    }

    void* res;

    //pthread_cancel()函数只是向线程发送一个取消请求，并不会立即终止线程的执行。
    //线程会在下一个取消点检查是否有取消请求，如果有，则会终止线程的执行。
    //因此，子线程可能会继续执行一段时间，直到它到达下一个取消点。
    //join会挂起 一直等待子线程结束 直到子线程结束后才会继续执行
    pthread_join(tid, &res);

    if (res == PTHREAD_CANCELED)
    {
        printf("线程被取消\n");
    }
    else
    {
        printf("线程正常退出 exit code %ld\n", (long)res);
    }
    return 0;
}