#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define handle_error(cmd, result) \
    if (result < 0)               \
    {                             \
        perror(cmd);              \
        return -1;                \
    }

void *read_from_server(void *arg)
{
    int sockfd = *(int *)arg;
    char *read_buf = NULL;
    ssize_t count = 0;

    read_buf = malloc(sizeof(char) * 1024);
    if (!read_buf)
    {
        perror("malloc client read_buf");
        return NULL;
    }

    // 接收数据，recv 返回 0 表示服务端关闭连接
    while ((count = recv(sockfd, read_buf, 1023, 0)) > 0)
    {
        read_buf[count] = '\0';
        fputs(read_buf, stdout);
    }

    if (count < 0)
    {
        perror("recv");
    }

    printf("收到服务端的终止信号......\n");
    free(read_buf);
    return NULL;
}

void *write_to_server(void *arg)
{
    int sockfd = *(int *)arg;
    char *write_buf = NULL;
    ssize_t send_count = 0;

    write_buf = malloc(sizeof(char) * 1024);
    if (!write_buf)
    {
        perror("malloc client write_buf");
        return NULL;
    }

    // 从标准输入读取数据并发送给服务端
    while (fgets(write_buf, 1024, stdin) != NULL)
    {
        send_count = send(sockfd, write_buf, strlen(write_buf), 0);
        if (send_count < 0)
        {
            perror("send");
            break;
        }
    }

    printf("接收到命令行的终止信号，不再写入，关闭连接......\n");
    shutdown(sockfd, SHUT_WR);
    free(write_buf);
    return NULL;
}

int main(int argc, char const *argv[])
{
    int sockfd, temp_result;
    pthread_t pid_read, pid_write;

    struct sockaddr_in server_addr;

    memset(&server_addr, 0, sizeof(server_addr));

    // 填写服务端地址
    server_addr.sin_family = AF_INET;
    // 连接本机 127.0.0.1
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    // 连接端口 8080
    server_addr.sin_port = htons(8080);

    // 1. 创建 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    handle_error("socket", sockfd);

    //2.绑定
    //客户端可以不绑定 系统会自动分配一个空闲的端口号给客户端

    // 2. 连接服务端
    temp_result = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (temp_result < 0)
    {
        perror("connect");
        close(sockfd);
        return -1;
    }

    printf("成功连接到服务端 127.0.0.1:8080\n");

    // 3. 启动读线程：读取服务端数据并打印到 stdout
    temp_result = pthread_create(&pid_read, NULL, read_from_server, (void *)&sockfd);
    if (temp_result != 0)
    {
        perror("pthread_create read");
        close(sockfd);
        return -1;
    }

    // 4. 启动写线程：从命令行读取数据并发送到服务端
    temp_result = pthread_create(&pid_write, NULL, write_to_server, (void *)&sockfd);
    if (temp_result != 0)
    {
        perror("pthread_create write");
        pthread_cancel(pid_read);
        close(sockfd);
        return -1;
    }

    // 5. 主线程等待子线程退出
    pthread_join(pid_read, NULL);
    pthread_join(pid_write, NULL);

    printf("关闭资源\n");
    close(sockfd);

    return 0;
}
