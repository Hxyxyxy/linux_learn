#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <string.h>

int main()
{
      char *share;
      pid_t pid;
      char shmName[100]={0};
      sprintf(shmName,"/letter%d",getpid());
      printf("shmName: %s\n", shmName);
      // 共享内存对象的文件标识符
      int fd;
      fd = shm_open(shmName, O_CREAT | O_RDWR, 0644);
      if (fd < 0)
      {
            perror("共享内存对象开启失败!\n");
            exit(EXIT_FAILURE);
      }
      while(1);
      return 0;
}