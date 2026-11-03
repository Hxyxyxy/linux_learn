#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>

int main()
{
    FILE *file = fopen("/home/hxy/linux_learn/pro4_socket_test/testfile.txt", "w");
    
    
    
    if (file == NULL)
    {
        perror("Failed to open file");
        return 1;
    }

    // 设置文件为行缓冲模式
    if (setvbuf(file, NULL, _IONBF, 0) != 0)
    {
        perror("Failed to set buffer mode");
        return 1;
    }

    //文件写入是全缓冲 等待缓冲区满或者手动刷新
    fprintf(file, "hello");

    char *argv[] = {"/usr/bin/ping", "-c", "1", "www.baidu.com", NULL};
    char *envp[] = {NULL};

    execve("/usr/bin/ping", argv, envp);

    //如果跳转失败
    perror("execve");

    return 0;
}