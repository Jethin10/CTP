#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/select.h>
#include <errno.h>
#include "../include/packet.h"
#include "../include/comm.h"

void send_frame(int fd, frame_t *f)
{
    if (fd < 0 || f == NULL) {
        return;
    }

    ssize_t written = write(fd, f, sizeof(frame_t));
    if (written < 0) {
        perror("write failed");
        return;
    }

    if (written != sizeof(frame_t)) {
        fprintf(stderr, "Warning: partial write\n");
        return;
    }

    printf("Sender: Data sent successfully!\n");
}

int receive_frame(int fd, frame_t *f)
{
    uint8_t buffer[sizeof(frame_t)];
    size_t bytes_received = 0;
    int preamble_found = 0;

    while (bytes_received < sizeof(frame_t)) {
        fd_set read_fds;
        struct timeval timeout;

        FD_ZERO(&read_fds);
        FD_SET(fd, &read_fds);
        timeout.tv_sec = 1;
        timeout.tv_usec = 0;

        int ready = select(fd + 1, &read_fds, NULL, NULL, &timeout);
        if (ready <= 0) {
            continue;
        }

        ssize_t n = read(fd, &buffer[bytes_received], 1);
        if (n < 0) {
            perror("read failed");
            return 0;
        }

        if (n == 0) {
            continue;
        }

        if (!preamble_found) {
            if (buffer[bytes_received] == 0xAA) {
                preamble_found = 1;
            }
        } else {
            bytes_received++;
        }
    }

    memcpy(f, buffer, sizeof(frame_t));
    printf("Receiver: Message read successfully\n");
    return 1;
}