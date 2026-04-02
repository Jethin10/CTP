#include <termios.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include "../include/termios.h"


int setup_termios(char* port_name){
    int fd = open(port_name, O_RDWR | O_NOCTTY);
    if(fd < 0){
        fprintf(stderr, "Error: Failed to open port %s: %s\n", port_name, strerror(errno));
        return -1;
    }

    struct termios tty;

    //current port setup
    if(tcgetattr(fd, &tty) < 0){
        fprintf(stderr, "Error: Failed to get terminal attributes: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    //baud rate
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    //character size - 8bits per character
    tty.c_cflag = (tty.c_cflag & ~CSIZE) | CS8;

    tty.c_iflag = 0;
    tty.c_oflag = 0;
    tty.c_lflag = 0;

    if(tcsetattr(fd, TCSANOW, &tty) < 0){
        fprintf(stderr, "Error: Failed to set terminal attributes: %s\n", strerror(errno));
        close(fd);
        return -1;
    }

    return fd;

}