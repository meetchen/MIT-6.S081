#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{

    int ftc[2],ctf[2];
    pipe(ftc), pipe(ctf);

    char buf[128] = {0};

    if (fork() == 0)
    {
        close(ftc[1]), close(ctf[0]);
        read(ftc[0], buf, 128);
        printf("%d: received %s\n", getpid(), buf);
        write(ctf[1],"pong", 5);
        exit(0);
        close(ftc[0]), close(ctf[1]);
    }
    close(ftc[0]), close(ctf[1]);

    write(ftc[1], "ping", 5);
    read(ctf[0], buf, 128);

    close(ftc[0]), close(ctf[1]);

    printf("%d: received %s\n", getpid(), buf);

    exit(0);
}
