#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>

#define SOCKET_PATH "unix_domain.socket"
#define SERVER_MODE 1
#define CLIENT_MODE 2
#define BUF_LEN 1024

static struct sockaddr_un socket_addr;
static char *buf;

void handle_error(char *err_msg)
{
    perror(err_msg);
    unlink(SOCKET_PATH);
    exit(-1);
}

void server_mode(int sockfd)
{
    //服务端接收消息 收到EOF后关闭连接
    int client_fd, msg_len;
    static struct sockaddr_un client_addr;
    if (bind(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0)
    {
        handle_error("bind");
    }

    if (listen(sockfd, 128) < 0)
    {
        handle_error("listen");
    }

    socklen_t client_addr_len = sizeof(client_addr);

    if ((client_fd = accept(sockfd, (struct sockaddr *)&client_addr, &client_addr_len)) < 0)
    {
        handle_error("accept");
    }

    write(STDOUT_FILENO, "Connected to client!\n", 21);

    do
    {
        memset(buf, 0, BUF_LEN);
        msg_len = recv(client_fd, buf, BUF_LEN, 0);
        
        if (strncmp(buf, "EOF", 3) != 0)
        {
            printf("Received msg: %s", buf);
            strcpy(buf, "OK!\n\0");
            send(client_fd, buf, strlen(buf), 0);
        }
        else
        {
            printf("Received EOF, exiting...\n");
            break;
        }

        //回复OK或者EOF
        send(client_fd, buf, strlen(buf), 0);
        
    } while (strncmp(buf, "EOF", 3) != 0);

    if (shutdown(client_fd, SHUT_RDWR) < 0)
    {
        handle_error("shutdown server");
    }
    unlink(SOCKET_PATH);
}

void client_mode(int sockfd)
{
    //客户端发送消息 收到EOF后关闭连接
    //连接服务端
    int msg_len, header_len;
    if (connect(sockfd, (struct sockaddr *)&socket_addr, sizeof(socket_addr)) < 0)
    {
        handle_error("connect");
    }

    write(STDOUT_FILENO, "Connected to server!\n", 21);
    strcpy(buf, "Msg received: ");

    // 计算buf中头的长度
    header_len = strlen(buf);

    do
    {
        printf("请输入要发送的消息: ");
        msg_len = read(STDIN_FILENO, buf + header_len, BUF_LEN - header_len);
        send(sockfd, buf + header_len, msg_len, 0);
        msg_len = recv(sockfd, buf + header_len, BUF_LEN - header_len, 0);
        write(STDOUT_FILENO, buf, msg_len + header_len);

        printf("接收到服务端回复的 消息\n");
    } while (strncmp(buf + header_len, "EOF", 3) != 0);
    
    shutdown(sockfd, SHUT_RDWR);
}

int main(int argc, char const *argv[])
{
    //设计结构
    //通过main方法传参的方式 启动不通的客户端和服务端
    //如果不填写参数 或者填写server 则启动服务端
    int fd = 0, mode = 0;

    if (argc == 1 || strncmp(argv[1], "server", 6) == 0)
    {
        mode = SERVER_MODE;
    }
    else if (strncmp(argv[1], "client", 6) == 0)
    {
        mode = CLIENT_MODE;
    }
    else
    {
        perror("参数错误");
        exit(-1);
    }

    //创建socket ipc通讯
    // address初始化
    memset(&socket_addr, 0, sizeof(struct sockaddr_un));
    buf = malloc(BUF_LEN);

    // 给address赋值
    socket_addr.sun_family = AF_UNIX;
    strcpy(socket_addr.sun_path, SOCKET_PATH);

    if ((fd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0)
    {
        handle_error("socket");
    }

    // 分服务端和客户端
    switch (mode)
    {
    case SERVER_MODE:
        server_mode(fd);
        break;
    case CLIENT_MODE:
        client_mode(fd);
        break;
    }

    if (shutdown(fd, SHUT_RDWR) < 0)
    {
        handle_error("shutdown");
    }
    free(buf);

    exit(0);
}