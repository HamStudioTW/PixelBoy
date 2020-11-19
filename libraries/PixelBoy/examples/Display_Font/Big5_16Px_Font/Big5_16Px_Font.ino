// PixelBoy Example: Chinese Font Big5-16x16 Test
// 

#include "b5f16.h"

//#define double_buffer

#include <PxMatrix.h>
#define P_LAT 22
#define P_A 2
#define P_B 19
#define P_C 25
#define P_D 26
#define P_E 21
#define P_OE 5
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);

void IRAM_ATTR display_updater() {
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(1);
  portEXIT_CRITICAL_ISR(&timerMux);
}

uint16_t _cx, _cy, _color1, color[8];
uint8_t bits[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};

uint32_t b5offset(uint16_t ch)
{
    uint32_t x1, x2, n;
    x1=ch>>8; x2=ch&0xff;
    n=(x1-161)*5024+362;
    if (x2<127) n+=(x2-64)*32;
    else n+=(x2-98)*32;
    return(n);
}

void showch(uint16_t b5, int x, int y, int c) 
{
    for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++)
            if (b5f16[b5offset(b5)+2*i] & bits[j]) display.drawPixel(x+j, y+i, c); 
            else display.drawPixel(x+j, y+i, 0);
        for(int j=0; j<8; j++) 
            if (b5f16[b5offset(b5)+2*i+1] & bits[j]) display.drawPixel(x+j+8, y+i, c);
            else display.drawPixel(x+j+8, y+i, 0);
    }
}

void showfont16(int f, int x, int y, int c)
{
    for(int i=0; i<16; i++) {
        for(int j=0; j<8; j++)
            if (b5f16[f*32+2*i+40] & bits[j]) display.drawPixel(x+j, y+i, c); 
            else display.drawPixel(x+j, y+i, 0);
        for(int j=0; j<8; j++) 
            if (b5f16[f*32+2*i+1+40] & bits[j]) display.drawPixel(x+j+8, y+i, c);
            else display.drawPixel(x+j+8, y+i, 0);
    }
}

void setup() 
{
  display.begin(16, 18, 23, -1, 0);
  display.setFastUpdate(true);
  display.setDriverChip(FM6124);
  display.flushDisplay();
  display.setTextWrap(false);
  display.setBrightness(10);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &display_updater, true);
  timerAlarmWrite(timer, 4000, true);
  timerAlarmEnable(timer);

  for(int i=0; i<8; i++) color[i]=rand()%65536;
  
  for(int i=64; i>-128; i--) {
      display.fillScreen(0);
      showch(0xB5BA, i+0, 16, color[4]); 
      showch(0xA4F5, i+16, 0, color[1]); 
      showch(0xB16D, i+16, 16, color[5]);
      showch(0xBB4C, i+32, 0, color[2]); 
      showch(0xBF4F, i+32, 16, color[6]);
      showch(0xAA4F, i+48, 16, color[7]);
      display.showBuffer();
      delay(80);
  }

}

void loop() 
{
  static int fn=40;

  _cx=(rand()%4)*16;
  _cy=(rand()%2)*16;
  _color1=(rand()%65536); 
  showfont16(fn++, _cx, _cy, _color1); 
  if (fn>13000) fn=40;
  
  // 送上燈板顯示
  display.showBuffer();
  
  // 延遲 20ms (50fps)
  delay(100);
}
