#include <time.h>
#include <mqueue.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    // 创建消息队列
    struct mq_attr attr;
    // 有用的参数    表示消息队列的容量
    attr.mq_maxmsg = 10;
    attr.mq_msgsize = 100;
    // 被忽略的消息 在创建消息队列的时候用不到
    attr.mq_flags = 0;
    attr.mq_curmsgs = 0;

    char *mq_name = "/p_c_mq";
    mqd_t mqdes = mq_open(mq_name, O_RDWR | O_CREAT, 0664, &attr);

    if (mqdes == (mqd_t)-1)
    {
        perror("mq_open");
        exit(EXIT_FAILURE);
    }

    //不断接收控制台输入的数据 发送到消息队列中
    char writeBuf[100];
    struct timespec time_info;


    while (1)
    {
        // 清空写缓冲区
        memset(writeBuf, 0, 100);

        // 从命令行标准输入读取数据
        ssize_t read_count = read(0, writeBuf, 100);

        // 获取当前时间中5s之后的timespec对象
        clock_gettime(CLOCK_REALTIME, &time_info);
        time_info.tv_sec += 5;

        //如果出错 报错
        if (read_count == -1)
        {
            perror("read");
            continue;
        }
        // 发送数据
        // 如果接收到命令行的EOF，read将返回0，此时向消费者端发送信息并退出
        else if (read_count == 0)
        {
            //若果从控制台接收到停止发送的消息
            //ctrl+d关闭控制台输入
            printf("Received EOF, exit...\n");
            char eof = EOF;
            //将EOF当做一条消息发送到消息队列中
            if (mq_timedsend(mqdes, &eof, 1, 0, &time_info) == -1)
            {
                perror("mq_timedsend");
            }
            break;
        }

        // 没有接收到EOF，正常发送数据
        if (mq_timedsend(mqdes, writeBuf, strlen(writeBuf), 0, &time_info) == -1)
        {
            perror("mq_timesend");
        }
        printf("从命令行接收到数据，已发送至消费者端\n");
    }

    // 关闭描述符
    close(mqdes);

    // mq_unlink 只应调用一次，我们选择在消费者中完成此操作

    return 0;
}