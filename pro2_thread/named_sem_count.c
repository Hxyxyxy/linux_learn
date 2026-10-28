#include <stdio.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    // char *shm_name = "unnamed_sem_shm";

    // // 创建内存共享对象
    // int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    // // 调整内存共享对象的大小
    // ftruncate(fd, sizeof(sem_t));
    // // 将内存共享对象映射到共享内存区域
    // sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // // 初始化信号量
    // sem_init(sem, 1, 0);

    //初始化有名信号量
    char *sem_name = "/named_sem_shm_value";
    sem_t *sem = sem_open(sem_name, O_CREAT, 0666, 0);

    int pid = fork();

    if (pid > 0)
    {
        //子进程睡眠1s 保证父进程先执行
        //sem_wait()  sem = 0无法减一，进入阻塞状态
        sem_wait(sem);
        printf("这是父进程\n");
        // 父进程等待子进程退出并回收资源
        waitpid(pid, NULL, 0);
    }
    else if (pid == 0)
    {
        // 子进程执行的代码
        sleep(1);
        printf("这是子进程\n");
        //子进程释放信号量，使sem+1 此时 父进程能够获取sem信号量，继续执行
        sem_post(sem);
    }
    else
    {
        perror("fork");
    }

    //回收资源
    //父子进程都需要解除引用
    if (sem_close(sem) == -1)
    {
        perror("sem_close");
    }
    if (pid > 0)
    {
        if (sem_unlink(sem_name) == -1)
        {
            perror("sem_unlink");
        }

    }
    

    return 0;
}