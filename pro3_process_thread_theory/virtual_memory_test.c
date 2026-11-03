#include <stdio.h>
#include <unistd.h>

int main(int argc, char const *argv[])
{
    int val = 123;
    __pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork failed");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        printf("Child process: val = %d\n", val);
        val = 456; // Modify the value in the child process
        printf("子进程中val的值是%d，地址值是%p\n", val, &val);
    }
    else
    {
        // Parent process
        printf("Parent process: val = %d\n", val);
        sleep(1); // Ensure child runs first
        printf("父进程中val的值是%d，地址值是%p\n", val, &val);
    }
    return 0;
}