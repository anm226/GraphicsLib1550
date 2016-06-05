/*Name: Andrew Masih
 *Email: anm226@pitt.edu
 *Project 1: Graphics Library
 */
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <termios.h>
#include <sys/select.h>

#define DEVICE "/dev/fb0"

int fd;
int ret;
int *mapped_file;
int file_size;

typedef unsigned short color_t;


int main(){

  init_graphics();

  draw_pixel();

  exit_graphics();


}
int init_graphics(){

  /* Open frame buffer device to read
   * if it fails to open, exit!
   */
  fd = open(DEVICE, O_RDWR);
  if (fd == -1) {
	   perror("Error opening file!");
	    exit(EXIT_FAILURE);
    }
  //get resolution info for mapped_file size
  struct fb_var_screeninfo info_about_virtual;
  ret = ioctl(fd, FBIOGET_VSCREENINFO, &info_about_virtual);
  if(ret == -1){
      perror("Encountered error trying to get virtual resolution!");
      exit(EXIT_FAILURE);
  }
  printf("This is screen info: %d", info_about_virtual.yres_virtual);
  //get screen infor for mapped_file size
  struct fb_fix_screeninfo bit_depth_info;
  ret = ioctl(fd, FBIOGET_FSCREENINFO, &bit_depth_info);
  if(ret == -1){
    perror("Encountered error trying to get fix screen info!");
    exit(EXIT_FAILURE);
  }
  printf("This is depth info: %d", bit_depth_info.line_length);
  //set mapped file size
  file_size = (info_about_virtual.yres_virtual)*(bit_depth_info.line_length);
  //map the file to address space
  mapped_file = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  //disable keypress echo and buffering keypresses using ioctl
  struct termios termOld;
  ret = ioctl(0,TCGETS,&termOld);
  if(ret==-1){
    perror("Error getting termios info!");
    exit(EXIT_FAILURE);
  }
  struct termios termNew;
  termNew = termOld;
  termNew.c_lflag &= ~ICANON; //turn off ICANON
  termNew.c_lflag &= ~ECHO; //turn off ECHO

  ret = ioctl(0, TCSETS, &termNew);
  if(ret==-1){
    perror("Error setting termios info!");
    exit(EXIT_FAILURE);
  }

}

/*Close all opened files, and enable
 *all the disabled functions
 */
int exit_graphics(){
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
  ret = munmap(mapped_file, file_size);
  if ( ret == -1) {
          perror("munmap failed with error:");
          exit(EXIT_FAILURE);
       }
  close(fd);
}
int getKey(){

  fd_set checkInput;

  //waits to see if the is input from stdin
  FD_ZERO(&checkInput);
  FD_SET(0, &checkInput);

  ret = select(1, &checkInput, NULL, NULL, NULL);

  if(ret==-1){
    perror("Failure from select call!");
    exit(EXIT_FAILURE);
  }
  char input[1];
  ret = read(0, input, sizeof(input));
  if(ret==-1){
    perror("Read failure!");
    exit(EXIT_FAILURE);
  }



}
//function to clear the screen
int clear_screen(){
 const char cmd[] = "\033[2J";
 write(1, cmd, sizeof(cmd)-1); //write out the string to clear screen!
}



int sleep_ms(){


}
int draw_pixel(){
  *(mapped_file + 25 + 5) = 255;
}

int draw_rect(){


}
int draw_text(){


}
