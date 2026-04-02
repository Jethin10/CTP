#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<sys/select.h>
#include<errno.h>
#include "../include/packet.h"
#include "../include/comm.h"

#define FRAME_WAIT_PREAMBLE 0
#define FRAME_READING       1
#define MAX_RETRIES        3

void send_frame(int fd, frame_t *f){
    if(fd < 0 || f == NULL){
        fprintf(stderr, "Error: Invalid parameters to send_frame\n");
        return;
    }
    
    ssize_t bytes_written = write(fd, f, sizeof(*f));
    if(bytes_written < 0){
        fprintf(stderr, "Error: Failed to send data: %s\n", strerror(errno));
    }else if(bytes_written < sizeof(*f)){
        fprintf(stderr, "Warning: Partial write (%zd/%zu bytes)\n", bytes_written, sizeof(*f));
    }else{
        printf("Sender: Data sent successfully!\n");
    }
}

static ssize_t read_with_timeout(int fd, void *buf, size_t count, int timeout_sec){
    fd_set read_fds;
    struct timeval tv;
    FD_ZERO(&read_fds);
    FD_SET(fd, &read_fds);
    tv.tv_sec = timeout_sec;
    tv.tv_usec = 0;
    
    int ret = select(fd + 1, &read_fds, NULL, NULL, &tv);
    if(ret <= 0){
        return ret;
    }
    return read(fd, buf, count);
}

int receive_frame(int fd, frame_t *f){
    uint8_t buffer[sizeof(frame_t)];
    size_t pos = 0;
    int state = FRAME_WAIT_PREAMBLE;
    int timeout_count = 0;
    const int max_timeouts = 10;
    
    memset(f, 0, sizeof(frame_t));
    memset(buffer, 0, sizeof(buffer));
    
    while(timeout_count < max_timeouts){
        ssize_t bytes_read = read_with_timeout(fd, &buffer[pos], 1, 1);
        
        if(bytes_read < 0){
            printf("Receiver: Error reading from serial port\n");
            return 0;
        }
        
        if(bytes_read == 0){
            timeout_count++;
            continue;
        }
        
        timeout_count = 0;
        
        if(state == FRAME_WAIT_PREAMBLE){
            if(buffer[pos] == 0xAA){
                state = FRAME_READING;
                pos++;
            }
        }
        else if(state == FRAME_READING){
            pos++;
            if(pos >= sizeof(frame_t)){
                memcpy(f, buffer, sizeof(frame_t));
                printf("Receiver: Message Read Successfully\n");
                return 1;
            }
        }
    }
    
    printf("Receiver: Timeout waiting for frame\n");
    return 0;
}


