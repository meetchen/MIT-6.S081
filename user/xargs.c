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

    memcpy(cmd, argv[1], sizeof(argv[1]));

    for (int i = 1; i < argc; i++)
    {
        _argv[i - 1] = argv[i];
    }

    int cur = 0, idx = argc - 1;
    _argv[idx] = malloc(128);

    while (read(STDIN, buf,1) == 1)
    {
        switch (buf[0])
        {
            case '\n':
            {
                if (fork() == 0)
                {
                    exec(cmd, _argv);
                    exit(0);
                }
                wait((int *) 0);
                // 是接受标准输入作为参数，是拼接在原有的参数后面，不能删除原来的参数
                // 我第一次把之前的参数删除了，。。。
                for (int i = argc - 1; i <= idx; i++)
                {
                    memset(_argv[i], 0, 128);
                    free(_argv[i]);
                }
                idx = argc - 1;
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