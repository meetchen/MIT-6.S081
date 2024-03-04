#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int find(char *path, char *dest);
int trimRight(char *name);

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    trimRight(buf);
    return buf;
}

int trimRight(char *name)
{
    for (int i = DIRSIZ - 1; i > -1; i--)
    {
        if (i != DIRSIZ && name[i] != ' ')
        {
            name[i + 1] = '\0';
            return 0;
        }
    }
    return 0;
}

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        printf("argv is not adequacy\n");
        exit(0);
    }
    find(argv[1], argv[2]);
    exit(0);
}

int find(char *path, char *dest)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        return 0;
    }

    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        return 0;
    }

    switch (st.type)
    {
    case T_FILE:
        if (strcmp(fmtname(path), dest) == 0)
        {
            printf("%s \n", path);
        }
        break;

    case T_DIR:
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
        {
            printf("find: path too long\n");
            break;
        }
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buf, &st) < 0)
            {
                printf("find: cannot stat %s\n", buf);
                continue;
            }
            char *name = fmtname(buf);
            if (strcmp(name, dest) == 0)
            {
                printf("%s \n", buf);
            }

            if (st.type == T_DIR && strcmp(name, ".") != 0 && strcmp(name, "..") != 0)
            {
                find(buf, dest);
            }
        }
        break;
    }
    close(fd);
    return 0;
}
