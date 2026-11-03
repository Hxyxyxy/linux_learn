#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

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

    //2. 服务器绑定地址
    socklen_t server_addr_len = sizeof(server_addr);
    temp_result = bind(sockfd, (struct sockaddr *)&server_addr, server_addr_len);
    handle_error("bind", temp_result);

    //直接收发数据
    do
    {
        //接收数据存到缓存
        // 清空缓冲区
        memset(buf, 0, 1024);

        // 尝试接受数据
        socklen_t client_addr_len = sizeof(client_addr);
        temp_result = recvfrom(sockfd, buf, 1024, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        handle_error("recvfrom", temp_result);

        // 如果客户端发来的不是EOF
        if (strncmp(buf, "EOF", 3) != 0)
        {
            printf("received msg from %s at port %d: %s", inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port), buf);
            strcpy(buf, "OK\n");
        }
        else
        {
            printf("received EOF from client, existing...\n");
        }

        // 收到数据后回复OK或EOF(客户端发送的是EOF则回复EOF，否则回复OK)
        temp_result = sendto(sockfd, buf, 4, 0, (struct sockaddr *)&client_addr, client_addr_len);
        handle_error("sendto", temp_result);
    } while (strncmp(buf, "EOF", 3) != 0);

    free(buf);

    return 0;
}