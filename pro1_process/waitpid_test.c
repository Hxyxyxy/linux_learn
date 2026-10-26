#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char const *argv[])
{
    //fork之前
    int subprocess_status;
    printf("老学员在校区\n");

    pid_t pid = fork();
    if(pid < 0)
    {
        perror("fork");
        return 1;
    }
    else if(pid == 0)
    {
        //新学员在这里
        char *args[] = {"/usr/bin/ping", "-c", "10", "www.baidu.com", NULL};
        char *envs[] = {NULL};
        printf("新学员%d联系海哥10次\n", getpid());

        int exR = execve(args[0], args, envs);
        
        if (exR < 0)
        {
            /* code */
            perror("execve");
            return 1;
        }
        
    }
    else
    {
        //老学员在这里
        printf("老学员%d等待新学员%d\n", getpid(), pid);
        waitpid(pid, &subprocess_status, 0);
        
    }
    printf("老学员%d等待新学员%d结束\n", getpid(), pid);

    return 0;
}