//
// PixelBoy Example: Double Buffer Animation Demo
// 
#include "pixelboy.h"

struct Text {
	char *text;
	uint16_t width, height;
	int16_t x, y;
	int16_t dx, dy;
} text1 = {"Pixel", 0, 0, 0, 0, 1, 1}, text2 = {"Boy", 0, 0, 30, 0, -1, 1};

void setup() 
{
  int16_t x1 = 0, y1 = 0;
  
  init_pixelboy();

  display.getTextBounds(text1.text, 0, 0, &x1, &y1, &text1.width, &text1.height);
  display.getTextBounds(text2.text, 0, 0, &x1, &y1, &text2.width, &text2.height);
  text1.width-=2; text1.height-=2;
  text2.width-=2; text2.height-=2;
}

void loop() 
{
  static int16_t x=0, dx=1;
  
  display.fillScreen(wbBLACK); // 清除畫面

  // 畫兩條線
  if(x+dx>=display.width() || x+dx<0) dx=-dx;
  x+=dx;
  display.drawLine(x,0, display.width()-x-1, display.height()-1, wbYELLOW);
  display.drawLine(display.width()-x-1, 0, x, display.height()-1, wbRED);

  // 第一個字串的彈跳
  if(text1.x+text1.dx+text1.width>=display.width() || text1.x+text1.dx<0) text1.dx=-text1.dx;
  if(text1.y+text1.dy+text1.height>=display.height() || text1.y+text1.dy<0) text1.dy=-text1.dy;
  text1.x+=text1.dx; text1.y+=text1.dy;
  display.setTextColor(wbGREEN);
  display.setCursor(text1.x, text1.y);
  display.print(text1.text);

  // 第一個字串的彈跳
  if(text2.x+text2.dx+text2.width>=display.width() || text2.x+text2.dx<0) text2.dx=-text2.dx;
  if(text2.y+text2.dy+text2.height>=display.height() || text2.y+text2.dy<0) text2.dy=-text2.dy;
  text2.x+=text2.dx; text2.y+=text2.dy;
  display.setTextColor(wbNAVY);
  display.setCursor(text2.x, text2.y);
  display.print(text2.text);

  display.showBuffer(); // 送上燈板顯示
  delay(20); // 延遲 20ms (50fps)
}
