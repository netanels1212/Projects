#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int fd;
    char *myfifo = "/tmp/myfifo";
    mkfifo(myfifo, 0666);

    char arr1[80];

    while (1)
    {
        fd = open(myfifo, O_WRONLY);
        write(fd, "ping send to pong", strlen("ping send to pong") + 1);
        close(fd);

        fd = open(myfifo, O_RDONLY);
        read(fd, arr1, sizeof(arr1));
        printf("%s\n", arr1);
        close(fd);
        sleep(1);
    }

    return 0;
}
