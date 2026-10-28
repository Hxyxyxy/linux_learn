#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <string.h> 

//定义结构体接收线程结果
typedef struct thread_result
{
    char *p;
    int len;
} thread_result_t;

//如果一个进程的多个线程同时使用到标准的输入输出 
//会造成冲突的问题 导致只能又一个线程能接收到消息 另一个线程无法接收到消息

//红玫瑰执行的线程函数
void *red_thread(void *arg)
{
    //初始化Result结构体
    thread_result_t *result = 
        (thread_result_t *)malloc(sizeof(thread_result_t));

    //解析传递的参数
    char code = *(char *)arg;

    //声明存放读取消息的字符串
    char *ans = malloc(1024);
    while(1)
    {
        if (fgets(ans, 1024, stdin) == NULL)
        {
            //输入结束或出错，安全退出
            free(ans);
            char *redans = strdup("红玫瑰线程：未收到指定消息\n");
            result->p = redans;
            result->len = strlen(redans);
            pthread_exit((void *)result);
        }
        if(ans[0] == code)
        {
            //接收了回复的消息
            free(ans);
            printf("红玫瑰线程接收到了消息\n");
            char *redans = strdup("红玫瑰线程接收到了消息ya \n");
            result->p = redans;
            result->len = strlen(redans);

            //结束线程 返回故事结果
            pthread_exit((void *)result);
        }else
        {
            printf("红玫瑰线程没有接收到消息\n");
        }
    }
}

//白玫瑰执行的线程函数
void *white_thread(void *arg)
{
    //初始化Result结构体
    thread_result_t *result = 
        (thread_result_t *)malloc(sizeof(thread_result_t));

    //解析传递的参数
    char code = *(char *)arg;

    //声明存放读取消息的字符串
    char *ans = malloc(1024);
    while(1)
    {
        if (fgets(ans, 1024, stdin) == NULL)
        {
            //输入结束或出错，安全退出
            free(ans);
            char *whiteans = strdup("白玫瑰线程：未收到指定消息\n");
            result->p = whiteans;
            result->len = strlen(whiteans);
            pthread_exit((void *)result);
        }
        if(ans[0] == code)
        {
            //接收了回复的消息
            free(ans);
            printf("白玫瑰离开了，线程接收到了消息\n");
            char *whiteans = strdup("白玫瑰线程接收到了消息 \n");
            result->p = whiteans;
            result->len = strlen(whiteans);

            //结束线程 返回故事结果
            pthread_exit((void *)result);
        }else
        {
            printf("白玫瑰线程没有接收到消息\n");
        }
    }
}

int main(int argc, char const *argv[])
{
    //海哥回复红玫瑰与白玫瑰为依据
    //创建两个线程 -> 红玫瑰线程和白玫瑰线程

    pthread_t tid_red;
    pthread_t tid_white;

    char red_code = 'r';
    char white_code = 'w';

    //Result结构体指针 用于接收线程返回的结果
    thread_result_t *red_result = NULL;
    thread_result_t *white_result = NULL;

    //创建红玫瑰线程
    pthread_create(&tid_red, NULL, red_thread, &red_code);
    //创建白玫瑰线程
    pthread_create(&tid_white, NULL, white_thread, &white_code);


    //获取红玫瑰线程的返回值
    pthread_join(tid_red, (void **)&red_result);
    printf("红玫瑰的结局-线程返回的结果: %s, 长度: %d\n", red_result->p, red_result->len);
    //释放红玫瑰线程返回的结果内存
    free(red_result->p);
    free(red_result);


    //获取白玫瑰线程的返回值
    pthread_join(tid_white, (void **)&white_result);
    printf("白玫瑰的结局-线程返回的结果: %s, 长度: %d\n", white_result->p, white_result->len);
    //释放白玫瑰线程返回的结果内存
    free(white_result->p);
    free(white_result);

    return 0;
}