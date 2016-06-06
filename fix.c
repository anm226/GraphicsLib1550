#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <termios.h>
#include <sys/select.h>

int main(){

  struct termios current;
  ret = ioctl(0, TCGETS, &current);
  if(ret==-1){
    perror("Error getting termios info!");
    exit(EXIT_FAILURE);
  }
  struct termios newInfo;
  newInfo = current;
  newInfo.c_lflag |= ICANON; //enable ICANON
  newInfo.c_lflag |= ECHO; //enable ECHO

  ret = ioctl(0, TCSETS, &newInfo);
  if(ret==-1){
    perror("Error setting termios info!");
    exit(EXIT_FAILURE);
  }
}
