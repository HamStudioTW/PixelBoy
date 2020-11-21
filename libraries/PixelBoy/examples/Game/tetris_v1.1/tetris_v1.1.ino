//
// PixelBoy Example: Tetris Game Demo (OK:ESP32)
// 
#include "pixelboy.h"
#include "pxb_tetris.h"
#define double_buffer

uint8_t font_num[50]={
  0x06,0x09,0x09,0x09,0x06, 0x06,0x02,0x02,0x02,0x07, 0x0E,0x01,0x06,0x08,0x0F, 0x0E,0x01,0x0E,0x01,0x0E,
  0x0A,0x0A,0x0A,0x0F,0x02, 0x0F,0x08,0x0E,0x01,0x0E, 0x06,0x08,0x0E,0x09,0x06, 0x0F,0x01,0x02,0x02,0x02,
  0x06,0x09,0x07,0x09,0x06, 0x06,0x09,0x06,0x01,0x06
};
uint8_t bits[8]={128,64,32,16,8,4,2,1};

void show_digit(int digit, int x, int y, int color)
{
  for(int i=0; i<5; i++)
    for(int j=0; j<4; j++) 
      if (font_num[digit*5+i]&bits[4+j]) display.drawPixel(y+i, (31-x-j), color);
      else display.drawPixel(y+i, (31-x-j), 0);
}

void show_num(int num, int x, int y, int color)
{
  int n=num;
  for(int i=0; i<5; i++) {
    show_digit(n%10, x+(4-i)*5, y, color);
    n/=10;
  }
}

void setup() 
{
  init_pixelboy();
  //init_preferences();
  init_game();
}

void loop() 
{
  //display.fillScreen(123); // 清除畫面
  getkey();
  check_downkey();
  check_leftkey();
  check_rightkey();
  check_rotrkey();
  check_rotlkey();
  check_selectkey();
  draw_board();
  blocks_fall();
  draw_preview();
  display.drawLine(6,0,63,0,0x1000);
  display.drawLine(6,31,63,31,0x1000);
  display.drawLine(63,0,63,31,0x1000);
  show_num(pts, 0, 0, 123); // game score
  show_digit(level, 0, 0, 0x00c0);
  display.showBuffer(); // 送上燈板顯示
  delay(20); // 延遲 20ms (50fps)
}
