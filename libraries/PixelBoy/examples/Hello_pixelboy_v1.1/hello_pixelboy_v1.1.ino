//
// PixelBoy Example: Simple Hello Demo
// 
#include "pixelboy.h"

void setup() 
{
  init_pixelboy();

  display.fillScreen(wbBLACK); // 清除畫面

  display.setTextColor(wbCYAN);
  display.setCursor(6, 6);
  display.print("Hello, ");
  display.setTextColor(wbPINK);
  display.setCursor(6, 16);
  display.print("PixelBoy!");

  display.showBuffer(); // 送上燈板顯示
}

void loop() 
{
}
