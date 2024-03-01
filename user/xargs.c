#include "kernel/types.h"
#include "kernel/param.h"
#include "kernel/stat.h"
#include "user/user.h"

#define BUFSIZE 8
#define STDIN 0

int main(int argc, char *argv[])
{
    char buf[BUFSIZE];
    char *_argv[MAXARG] = {0};
    char cmd[MAXPATH];
    int idx = 0;

    memcpy(cmd, argv[1], sizeof(argv[1]));

    for (int i = 1; i < argc; i++)
    {
        _argv[i - 1] = argv[i];
        idx++;
    }

    int cur = 0;
    _argv[idx] = malloc(128);

    while (read(STDIN, buf,1) == 1)
    {
        // printf("-%c-\n", buf[0]);
        switch (buf[0])
        {

            case '\n':
            {
                // printf("_argv[0] %d: %s \n", strlen(_argv[0]), _argv[0]);
                // printf("_argv[1] %d: %s \n", strlen(_argv[1]), _argv[1]);
                // printf("_argv[2] %d: %s \n", strlen(_argv[2]), _argv[2]);
                // for (int i = idx; i < MAXARG; i++)
                // {
                //     free(_argv[i]);
                // }
                // printf("_argv[3] %d: %s \n", strlen(_argv[3]), _argv[3]);

                if (fork() == 0)
                {
                    exec(cmd, _argv);
                    exit(0);
                }
                wait((int *) 0);
                for (int i = 1; i < idx; i++)
                {
                    memset(_argv[i], 0, 128);
                    free(_argv[i]);
                }
                idx = 1;
                cur = 0;
                _argv[idx] = malloc(128);
                break;
            }

            case ' ':
            {
                idx++;
                _argv[idx] = malloc(128);
                cur = 0;
                break;
            }
                
            
            default:
            {
                _argv[idx][cur++] = buf[0];
                break;
            }
        }
    }


    exit(0);

}