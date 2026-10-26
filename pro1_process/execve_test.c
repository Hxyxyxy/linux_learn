#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char const *argv[])
{
    // 跳转之前
    char *name = "banzhang";
    printf("我是%s 编号%d 我现在在一楼\n", name, getpid());

    // 跳转之后
    char *args[] = {"/home/hxy/linux_learn/pro1_process/erlou", name, NULL};
    char *envs[] = {"/opt/ros/humble/bin:/home/hxy/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/snap/bin:/snap/bin", NULL};
    int re = execve(args[0], args, envs);
    int re = execve(args[0], args, envs);
    if (re == -1)
    {
        printf("跳转失败\n"); // 只有 execve 失败才执行
        return 1;
    }

    return 0; // 这一行也执行不到
}