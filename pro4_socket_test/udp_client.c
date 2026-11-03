#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

#define handle_error(cmd, result) \
    if (result < 0)               \
    {                             \
        perror(cmd);              \
        return -1;                \
    }

int main(int argc, char const *argv[])
{
    //使用 UDP 协议完成客服端和服务端通信
    //EOF作为关闭的信号
    int sockfd, temp_result, client_fd;
    struct sockaddr_in server_addr, client_addr;
    char *buf = malloc((sizeof(char)) * 1024);

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // 声明IPV4通信协议
    server_addr.sin_family = AF_INET;
    // 我们需要绑定0.0.0.0地址，转换成网络字节序后完成设置
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // 端口随便用一个，但是不要用特权端口
    server_addr.sin_port = htons(6666);

    //UDP编程流程
    // 创建server socket，注意通信类型
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    handle_error("socket", sockfd);

    //2. 客户端不需要绑定地址
    socklen_t server_addr_len = sizeof(server_addr);
    socklen_t client_addr_len = sizeof(client_addr);

    //直接收发数据
    do
    {
        //
        printf("please input msg to send to server: ");

        //从控制台读取数据
        int buf_len = read(STDIN_FILENO, buf, 1024);

        temp_result = sendto(sockfd, buf, buf_len, 0, (struct sockaddr *)&server_addr, server_addr_len);
        handle_error("sendto", temp_result);

        //清空缓冲区
        memset(buf, 0, 1024);

        temp_result = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        handle_error("recvfrom", temp_result);

        if (strncmp(buf, "EOF", 3) != 0)
        {
            printf("received msg from %s at port %d: %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
        }

    } while (strncmp(buf, "EOF", 3) != 0);

    free(buf);

    return 0;
}