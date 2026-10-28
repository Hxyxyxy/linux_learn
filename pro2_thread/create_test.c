#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define BUF_LEN 1024

char *buf;

//读线程需要执行的代码逻辑
void *input_thread(void *argv)
{
    int i = 0;
    while (1)
    {
        char c = fgetc(stdin);
        if (c && c != '\n')
        {
            buf[i++] = c;
        }
        //缓冲区索引溢出
        if (i >= BUF_LEN)
        {
            i = 0;
        }
    
    }
}

//写线程需要执行的代码逻辑
void *output_thread(void *argv)
{
    int i = 0;
    while (1)
    {
        if (buf[i])
        {
            //读取一个字节写出到控制台之后换行
            fputc(buf[i], stdout);
            fputc('\n', stdout);
            buf[i++] = 0;

            //缓冲区索引溢出
            //缓冲区索引溢出
            if (i >= BUF_LEN)
            {
                i = 0;
            }
        }else
        {
            //如果缓冲区没有数据，线程休眠1秒
            sleep(1);
        }
    }
}

//程序实现创建两个线程
// （1）读取控制台信息 写入到缓存区
// （2） 将缓存区信息写入到控制台
int main(int argc, char const *argv[])
{
    //初始化buf
    buf = malloc(BUF_LEN);
    //声明线程
    pthread_t tid_input;
    pthread_t tid_output;
    //创建读线程
    pthread_create(&tid_input, NULL, input_thread, NULL);
    //创建写线程
    pthread_create(&tid_output, NULL, output_thread, NULL);
    
    //主线程等待读写线程结束
    pthread_join(tid_input, NULL);
    pthread_join(tid_output, NULL);

    free(buf);
    
    return 0;
}
