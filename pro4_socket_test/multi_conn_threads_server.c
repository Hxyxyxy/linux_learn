#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <unistd.h>

#define handle_error(cmd, result) \
    if (result < 0)               \
    {                             \
        perror(cmd);              \
        return -1;                \
    }

void * read_from_client_then_write(void *argv)
{
    //读取客户端发送的数据 打印到控制台 回复收到
    int client_fd = *(int *)argv;
    char *read_buf = NULL;
    char *write_buf = NULL;
    ssize_t count = 0, send_count = 0;

    read_buf = malloc(sizeof(char) * 1024);
    write_buf = malloc(sizeof(char) * 1024);

    if(!read_buf)
    {
        printf("初始化读缓存失败\n");
        close(client_fd);
        perror("malloc server read_buf");
        return NULL;
    }

    if(!write_buf)
    {
        printf("初始化写缓存失败\n");
        free(read_buf);
        close(client_fd);
        perror("malloc server write_buf");
        return NULL;
    }

    while((count = recv(client_fd, read_buf, 1024, 0)))
    {
        if(count < 0)
        {
            perror("recv");
        }
        printf("接收到客户端 %d 的消息: %s\n", client_fd, read_buf);

        //接收数据打印到控制台
        printf("接收到客户端 %d 的消息: %s\n", client_fd, read_buf);

        //首先把收到的消息写到写缓存
        strcpy(write_buf, "收到消息~\n");
        send_count = send(client_fd, write_buf, 1024, 0);
        if(send_count < 0)
        {
            perror("send");
        }
    }

    //当客户端输入ctrl+d会退出循环
    // shutdown(client_fd, SHUT_WR);
    close(client_fd);
    free(read_buf);

}

int main(int argc, char const *argv[])
{
    int sockfd, temp_result;
   
    struct sockaddr_in server_addr, client_addr;

    // 清空
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // 填写服务端地址
    server_addr.sin_family = AF_INET;
    // 填写 ip 地址 0.0.0.0
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 填写端口号
    server_addr.sin_port = htons(8080);

    // 1. 创建 socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket");
        return -1;
    }

    // 2. 绑定地址
    temp_result = bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (temp_result < 0)
    {
        perror("bind");
        close(sockfd);
        return -1;
    }

    // 3. 进入监听状态
    temp_result = listen(sockfd, 128);
    if (temp_result < 0)
    {
        perror("listen");
        close(sockfd);
        return -1;
    }

    // 4. 获取客户端连接
    //需要能够接收多个连接
    socklen_t client_addr_len = sizeof(client_addr);
    while(1)
    {
        int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len);
        pthread_t tid_read_write;
        handle_error("accept", client_sockfd);
        printf("与客户端 %s %d 建立连接，文件描述符 %d\n",
               inet_ntoa(client_addr.sin_addr), 
               ntohs(client_addr.sin_port), client_sockfd);
        
        //和每一个客户端使用一个线程进行通信 把客户端发送的信息打印到控制台 回复收到
        if (pthread_create(&tid_read_write, NULL, read_from_client_then_write, &client_sockfd))
        {
            perror("pthread_create");
        }

        //需要等待线程结束 但不能挂起等待
        pthread_detach(tid_read_write);
        
    }
    
    printf("关闭资源\n");
    close(sockfd);

    return 0;
}
