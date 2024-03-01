#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int
main(int argc, char *argv[])
{

    if (argc < 2) 
    {
        write(1, "argv is not adequacy", sizeof("argv is not adequacy"));
        exit(1);
    }

    int len = atoi(argv[1]);
    if (len == -1)
    {
        write(1,"argv error", sizeof("argv error"));
        exit(1);
    }

    sleep(len);
    exit(0);
}
