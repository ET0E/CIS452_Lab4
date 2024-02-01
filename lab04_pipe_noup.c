#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <error.h>

#define READ 0
#define WRITE 1
#define MAX 1024

int main () {
    int fd[2];
    size_t num, len;
    pid_t pid;
    char str[MAX];

    if (pipe (fd) < 0) {
        perror ("plumbing problem");
        exit (1);
    }
    printf("Pipe descriptors: read=%d write=%d\n", fd[0], fd[1]);
    // point A

    if ((pid = fork ()) < 0) {
        perror ("fork failed");
        exit (1);
    }
    // point B
    else if (pid == 0) {
        close (fd[READ]);

        //point C
        printf ("Type a sentence: ");
        fgets (str, MAX, stdin);
        printf ("Sent by %d: %s", getpid(), str);
        len = strlen(str) + 1;
        write (fd[WRITE], &len, sizeof(len));
        write (fd[WRITE], (const void *) str, (size_t) len);
        exit (0);
    }


    close (fd[WRITE]);

    //point C
    read(fd[READ], &len, sizeof(len));
    num = read (fd[READ], (void *) str, len);
    if (num != len) {
        perror ("pipe read error\n");
        exit (1);
    }
    printf ("Received by %d: %s", getpid(), str);
    return 0;
}