#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"



int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("argv is not adequacy\n");
        exit(0);
    }

    int mask = atoi(argv[1]);

    // 获取需要跟踪的系统调用
    int trace_id = 1;
    while ((mask >>= 1) != 1)
    {
        trace_id++;
    }

    trace(trace_id);

    exit(0);
}