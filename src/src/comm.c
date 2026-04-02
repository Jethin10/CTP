#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>
#include<errno.h>
#include "../include/packet.h"
#include "../include/comm.h"

void send_frame(int fd, frame_t *f){
    if(fd < 0 || f == NULL){
        return;
    }

    ssize_t bytes_written = write(fd, f, sizeof(*f));
    if(bytes_written < 0){
        perror("write failed");
    }else if(bytes_written != sizeof(*f)){
        printf("Warning: partial write\n");
    }else{
        printf("Sender: Data sent successfully!\n");
    }
}

int receive_frame(int fd, frame_t *f){
    uint8_t buffer[sizeof(frame_t)];
    size_t pos = 0;
    int preamble_found = 0;

    memset(buffer, 0, sizeof(buffer));

    while(pos < sizeof(frame_t)){
        fd_set read_fds;
        struct timeval tv;
        FD_ZERO(&read_fds);
        FD_SET(fd, &read_fds);
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        int ret = select(fd + 1, &read_fds, NULL, NULL, &tv);
        if(ret <= 0){
            continue;
        }

        ssize_t n = read(fd, &buffer[pos], 1);
        if(n < 0){
            perror("read failed");
            return 0;
        }

        if(n == 0){
            continue;
        }

        if(!preamble_found){
            if(buffer[pos] == 0xAA){
                preamble_found = 1;
            }
        }else{
            pos++;
        }
    }

    memcpy(f, buffer, sizeof(frame_t));
    printf("Receiver: Message Read Successfully\n");
    return 1;
}