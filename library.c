//Andrew Masih
//anm226@pitt.edu
//Project 1: Graphics Library
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>

#define DEVICE "/dev/fb0"

int fd;

int init_graphics(){

  fd = open(DEVICE, O_RDWR);

}
int exit_graphics(){


}

int clear_screen(){


}

int getkey(){


}

int sleep_ms(){


}
int draw_pixel(){

}

int draw_rect(){


}
int draw_text(){


}
