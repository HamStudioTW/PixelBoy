// PixelBoy Example: Chinese Font Test
// 

#include "wbFontB12.h"
#include <pixelboy.h>
#include <PxMatrix.h>
uint8_t bits[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
uint8_t fbuf[18];
uint16_t _cx, _cy, _color1;
void showbits(uint8_t c, uint8_t n)
{
    for(int i=0; i<4; i++) {
        if (c & bits[i+n]) display.drawPixel(_cx, _cy, _color1);
        else display.drawPixel(_cx, _cy, 0);
        _cx+=1;
    }
}
void dispfont(uint16_t n)
{
     memcpy_P(fbuf, (uint8_t *)(&b5f12[n*18]), 18);
     int count = 0;
     for(int i=0; i<18; i++) {
         showbits(fbuf[i], 0);
         if (++count % 3 == 0) { _cx-=12; _cy+=1; } 
         showbits(fbuf[i], 4);
         if (++count % 3 == 0) { _cx-=12; _cy+=1; } 
     }
}

void setup() 
{
  init_pixelboy();
  display.fillScreen(display.color565(0,0,0));
}
void loop() 
{
  _cx=(rand()%5)*13;
  _cy=(rand()%2)*14+3;
  _color1=(rand()%65536); //|0x8410;
  dispfont(rand()%5401);
  display.showBuffer();// 送上燈板顯示
  delay(20);// 延遲 20ms (50fps)
}
