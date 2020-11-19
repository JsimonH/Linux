#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

int main()
{  
    int fd = open("./myfifo", O_WRONLY);
    if(fd < 0)
    {
        perror("open");
        return 0;
    }

    write(fd, "hello", 5);
    close(fd);
    return 0;
}
