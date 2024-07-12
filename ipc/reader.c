#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>

int main() {
    int fd = open("/dev/ipc_device", O_RDONLY);
    if (fd < 0) {
        perror("Failed to open the device");
        return -1;
    }

    char buffer[2048];
    int bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read < 0) {
        perror("Failed to read the message");
        return -1;
    }

    buffer[bytes_read] = '\0';
    printf("Received message: %s\n", buffer);
    close(fd);

    return 0;
}