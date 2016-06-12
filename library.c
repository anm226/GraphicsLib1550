/*Name: Andrew Masih
 *Email: anm226@pitt.edu
 *Project 1: Graphics Library
 */

#include <fcntl.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <termios.h>
#include <sys/select.h>
#include <time.h>
#include "iso_font.h"

#define DEVICE "/dev/fb0"

int fd;
int ret;
int *mapped_file;
int file_size;
struct fb_var_screeninfo info_about_virtual;
struct fb_fix_screeninfo bit_depth_info;
typedef unsigned short color_t;

void draw_text(int x, int y, const char *text, color_t  c);
void draw_rect(int x, int y, int width, int height, color_t c);

void init_graphics(){

  /* Open frame buffer device to read
   * if it fails to open, exit!
   */
  fd = open(DEVICE, O_RDWR);
  if (fd == -1) {
	   perror("Error opening file!");
    }
  //get resolution info for mapped_file size
  ret = ioctl(fd, FBIOGET_VSCREENINFO, &info_about_virtual);
  if(ret == -1){
      perror("Encountered error trying to get virtual resolution!");
  }

  //get screen infor for mapped_file size
  ret = ioctl(fd, FBIOGET_FSCREENINFO, &bit_depth_info);
  if(ret == -1){
    perror("Encountered error trying to get fix screen info!");
  }

  //set mapped file size
  file_size = (info_about_virtual.yres_virtual)*(bit_depth_info.line_length);
  //map the file to address space
  mapped_file = mmap(0, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

  //disable keypress echo and buffering keypresses using ioctl
  struct termios termOld;
  ret = ioctl(0,TCGETS,&termOld);
  if(ret==-1){
    perror("Error getting termios info!");
  }
  struct termios termNew;
  termNew = termOld;
  termNew.c_lflag &= ~ICANON; //turn off ICANON
  termNew.c_lflag &= ~ECHO; //turn off ECHO

  ret = ioctl(0, TCSETS, &termNew);
  if(ret==-1){
    perror("Error setting termios info!");

  }

}

/*Close all opened files, and enable
 *all the disabled functions
 */
void exit_graphics(){
  struct termios current;
  ret = ioctl(0, TCGETS, &current);
  if(ret==-1){
    perror("Error getting termios info!");

  }
  struct termios newInfo;
  newInfo = current;
  newInfo.c_lflag |= ICANON; //enable ICANON
  newInfo.c_lflag |= ECHO; //enable ECHO

  ret = ioctl(0, TCSETS, &newInfo);
  if(ret==-1){
    perror("Error setting termios info!");

  }
  ret = munmap(mapped_file, file_size);
  if ( ret == -1) {
          perror("munmap failed with error:");

       }
  close(fd);
}

//gets any keypresses
char getkey(){

  fd_set checkInput;

  //waits to see if the is input from stdin
  FD_ZERO(&checkInput);
  FD_SET(0, &checkInput);

  char input = 0;
  if(select(1, &checkInput, NULL, NULL, NULL)>0){
        if(read(0, &input, 1)<0){
          perror("read fail!");
        }
  }


  return input;
}
//function to clear the screen
void clear_screen(){
 const char cmd[] = "\033[2J";
 write(1, cmd, sizeof(cmd)-1); //write out the string to clear screen!
}

//draws pixel at given coordinates
void draw_pixel(int x, int y, color_t color){

  //casts mapped file to short, so it's possible to
  //go through it short by short since
  //there are 16 bits per pixel
  unsigned short *pixel = (unsigned short *)mapped_file;
    int scale = (y*640)+x;
    *(pixel + scale) = color;

}

//basic drawing rect function using pixels
void draw_rect(int x, int y, int width, int height, color_t c){
  int i;

  for(i = 0; i<width; i++){
    draw_pixel(x+i, y, c);
  }
  for(i = 0; i<height; i++){
    draw_pixel(x, y+i, c);
  }
  for(i=0; i<width; i++){
    draw_pixel(x+i, y+height, c);
  }
  for(i=0; i<=height; i++){
    draw_pixel(x+width, y+i, c);
  }

}

//sleep function, takes ms and * it by 1,000,000 to make ns
void sleep_ms(long ms){
  struct timespec t;
  t.tv_sec = 0;
  t.tv_nsec = ms*1000000L;

  if(nanosleep(&t, NULL)<0){
    perror("nanosleep() fail!");
  }
}

//draw_text function, reads char by char and writes out chars
void draw_text(int x, int y, const char *text, color_t  c){
  x = x+8;
  unsigned char reader;
  unsigned int mask;
  int ascii = 0;
  int counter = 0;
  int i;
  int move_x =0;

  char iterator = text[counter];

  while(iterator!='\0'){
    ascii = (int)iterator;
    for (i = 0; i < 16; i++) {
      reader = iso_font[ascii*16+i];

      int move_x =0;
      for (mask = 0x80; mask != 0; mask >>= 1) {

          if (reader & mask) {
              draw_pixel(x-move_x,y+i, c);
          }
          else {
          }
          move_x++;

      }

    }
    x = x+8;
    iterator = text[++counter];
  }


}
