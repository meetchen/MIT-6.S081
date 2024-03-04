#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int prime(int fd);

int
main(int argc, char *argv[])
{

    int p[2];
    pipe(p);

    if (fork())
    {
        close(p[0]);
        for (int i = 2; i < 35; i++)
        {
            write(p[1], &i, sizeof(i));
        }
        close(p[1]);
    }
    else 
    {
        close(p[1]);
        prime(p[0]);
    }
    wait((int*)0);
    close(p[0]);

    exit(0);
    
}

int prime(int fd)
{
    int base;
    if (read(fd, &base, sizeof(base)) == 0)
    {
        exit(0);
    }
    printf("prime %d\n", base);

    int p[2];
    pipe(p);

    if (fork())
    {
        close(p[0]);
        int cnt = 0, temp = 0;
        while ((cnt = read(fd, &temp, sizeof(temp))))
        {
            if (temp % base)
            {
                write(p[1], &temp, sizeof(temp));
            }
        }
        close(p[1]);
    }
    else
    {
        close(p[1]);
        prime(p[0]);
    }
    wait((int*)0);
    exit(0);
}
