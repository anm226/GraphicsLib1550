#include <stdio.h>
#include <stdlib.h>

typedef unsigned short color_t;

char key = 0;
int x = 10;
int y = (480-20)/2;

void clear_screen();
void exit_graphics();
void init_graphics();
char getkey();
void sleep_ms(long ms);

void draw_pixel(int x, int y, color_t color);
void draw_rect(int x1, int y1, int width, int height, color_t c);
void draw_text(int x2, int x3, const char *text, color_t  c);

void draw_banner(int x, int y){
  int x2 = x+2;
  int y2 = y+2;

  int x3 = x+180;
  int y3 = y+20;
  //Author: Andrew Masih
  draw_rect(x,y,180,20, 63488);
  draw_text(x2, y2, "Author: Andrew Masih!", 38904);
  int i;
  for(i =0; i<11; i++){
    draw_pixel(x3+i, y+i, 63488);
  }
  for(i =0; i<11; i++){
    draw_pixel(x3+i, y3-i, 63488);
  }
  x3 = x3+10;
  y3 = y3-10;
  for(i =0; i<6; i++){
    draw_pixel(x3+i,y3,65535);
  }
}
int main(){
  clear_screen();
  init_graphics();
  printf("Press any char key to see animation or press 'q' at any time to quit!\n");

  int pixel_mover = 1;
  while(key!='q'){
    key = getkey();
    draw_banner(x+pixel_mover,y);
    if((x+pixel_mover+195)==635){
      pixel_mover=1;
    }
    pixel_mover=pixel_mover+4;
    sleep_ms(40);
    clear_screen();
  }

  exit_graphics();

}
