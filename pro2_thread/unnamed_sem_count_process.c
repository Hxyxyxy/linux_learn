#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    char *shm_name = "unnamed_sem_shm";

    // 创建内存共享对象
    int fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
    // 调整内存共享对象的大小
    ftruncate(fd, sizeof(sem_t));
    // 将内存共享对象映射到共享内存区域
    sem_t *sem = mmap(NULL, sizeof(sem_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    // 初始化信号量
    sem_init(sem, 1, 0);

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

    // 父进程执行到此处，子进程已执行完毕，可以销毁信号量
    // 子进程执行到此处，父进程仍在等待信号量，此时销毁会导致未定义行为
    // 只有父进程中应该销毁信号量
    if (pid > 0)
    {
        if (sem_destroy(sem) == -1)
        {
            perror("father sem_destroy");
        }
    }

    // 父子进程都应该解除映射，关闭文件描述符
    if (munmap(sem, sizeof(sem)) == -1)
    {
        perror("munmap");
    }
    if (close(fd) == -1)
    {
        perror("close");
    }

    // shm_unlink只能调用一次，只在父进程中调用
    if (pid > 0)
    {
        if (shm_unlink(shm_name) == -1)
        {
            perror("father shm_unlink");
        }
    }

    return 0;
}