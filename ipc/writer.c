#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main() {
    int fd = open("/dev/ipc_device", O_WRONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }
    char buffer[2048];
    fgets(buffer, sizeof(buffer), stdin);
    write(fd, buffer, strlen(buffer));
    close(fd);

    return 0;
}
