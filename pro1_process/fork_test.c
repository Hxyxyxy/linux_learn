#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    //调用fork之前 代码都在父进程中进行
    printf("还个教老学院%d写情书\n", getpid());

    //使用fork创建子进程
    /**
     * 不需要传参
     */
    pid_t pid = fork();
    // printf("%d\n", pid);

    if(pid < 0){
        printf("新学员加入失败\n");
        return 1;
    }else if(pid == 0)
    {
        //执行单独代码
        printf("新学员%d加入成功，他是老学员%d推荐的\n", getpid(), getppid());
    }else{
        //执行单独副进程
        printf("老学员%d继续神造，他还推荐了%d\n", getpid(), pid);
    }

    return 0;
}