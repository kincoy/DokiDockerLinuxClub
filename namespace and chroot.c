#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>
#include <unistd.h>
#include <sched.h>
#define STACK_SIZE (1024*1024)
static char container_stack[STACK_SIZE]; /*设置子进程的堆栈空间大小*/


/*用来打印每个 ns 的相关信息*/
void printf_ns(void)
{
        system("readlink /proc/$$/ns/pid");
        system("readlink /proc/$$/ns/mnt");
        system("readlink /proc/$$/ns/net");
        system("readlink /proc/$$/ns/user");
        system("readlink /proc/$$/ns/ipc");
        system("readlink /proc/$$/ns/uts");
}

/*子进程启动的函数*/
int child_func(void *arc)
{
        /*用来打印提示和相关信息*/
        system("mount -t proc proc /proc");
        printf("-----------------------------------\n");
        printf("welcome to the new space!\n");

        /*设置下主机名*/
        sethostname("kinco", 5);
        printf("now the host name is kinco!\n");

        /*输出新的namespace信息*/
        printf("the new pid : %d \n", getpid());
        printf_ns();

        /*隔离文件系统，并启动shell*/
        system("cd /tmp/rootfs");
        chroot("/tmp/rootfs");
        system("/bin/bash");
        return 0;
}

int main(void)
{
        /*打印原来进程中的namespace信息*/
        printf("the old ns: \n");
        printf_ns();

        /*一个子进程，满足了五个ns隔离*/
        int clone_pid = clone(child_func,
                              container_stack + STACK_SIZE,
                              CLONE_NEWPID | CLONE_NEWNET | CLONE_NEWUTS | CLONE_NEWIPC | CLONE_NEWNS | SIGCHLD ,
                              NULL);
        if (clone_pid < 0)
            {
                printf("clone error!");
                exit(EXIT_FAILURE);
            }

        sleep(2);
        /*判断是否执行成功*/
        if(waitpid(clone_pid, NULL, 0) == -1)
            {
                printf("waitpid error!");
            exit(EXIT_FAILURE);
            }

            /*重新挂在一下*/
        system("mount -t proc proc /proc");

            /*提示子进程运行结束*/
        printf("back to the father.\n");

        return 0;
}

