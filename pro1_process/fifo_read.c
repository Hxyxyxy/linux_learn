#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

int main()
{
    int fd;
    char *pipe_path = "/tmp/myfifo";

    if (mkfifo(pipe_path, 0664) != 0)
    // {
    //     perror("mkfifo failed");
    //     if (errno != 17)
    //     {
    //         exit(EXIT_FAILURE);
    //     }
    // }

    // 打开有名管道用于读取
    fd = open(pipe_path, O_RDONLY);
    if (fd == -1)
    {
        perror("open failed");
        exit(EXIT_FAILURE);
    }

    char read_buf[100];
    ssize_t read_num;

    while ((read_num = read(fd , read_buf, 100)) > 0) {
        write(STDOUT_FILENO, read_buf, read_num);
    }

    if (read_num < 0) {
        perror("read");
        printf("命令行数据读取异常，退出");
        close(fd);
        exit(EXIT_FAILURE);
    }
    
    printf("接收管道数据完成，进程终止\n");
    close(fd);

    return 0;
}