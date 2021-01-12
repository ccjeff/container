#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <sched.h>
#include <signal.h>
#include <unistd.h>

#define STACK_SIZE (1024 * 1024)
static char child_stack[STACK_SIZE];

int spawn_bash(void)
{
    char *newargv[] = { "/bin/bash", NULL };

    execv("/bin/bash", newargv);
    perror("exec");
    exit(EXIT_FAILURE);
}


int child(void *args)
{
    printf("pid as seen in the child: %lu\n", (unsigned long)getpid());
    spawn_bash();
}


int main()
{
    // int namespaces = CLONE_NEWUTS|CLONE_NEWPID|CLONE_NEWIPC|CLONE_NEWNS|CLONE_NEWNET;
    int namespace = CLONE_NEWUTS;
    pid_t p = clone(child, malloc(4096) + 4096, SIGCHLD|namespace, NULL);
    if (p == -1) {
        perror("clone");
        exit(1);
    }
    printf("child pid: %u\n", p);
    waitpid(p, NULL, 0);
    return 0;
}