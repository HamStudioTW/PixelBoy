// PixelBoy Example: Chinese Font HZK16x16 Test
// 

#include "wbFontB12.h"
#include "HZK16.h"

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
  display.display(20);
  portEXIT_CRITICAL_ISR(&timerMux);
}

uint8_t bits[]={0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01};
uint8_t fbuf[32];
uint16_t _cx, _cy, _color1;

void showbits16(uint8_t c, uint8_t n)
{
    for(int i=0; i<8; i++) {
        if (c & bits[i+n]) display.drawPixel(_cx, _cy, _color1);
        else display.drawPixel(_cx, _cy, 0);
        _cx+=1;
    }
}

void dispfont16(uint16_t n)
{
     memcpy_P(fbuf, (uint8_t *)(&HZK16[n*32]), 32);
     int count = 0;
     for(int i=0; i<32; i++) {
         showbits16(fbuf[i], 0);
         if (++count % 2 == 0) { _cx-=16; _cy+=1; } 
     }
}

void setup() 
{
  display.begin(16, 18, 23, -1, 0);
  display.setFastUpdate(true);
  display.setDriverChip(FM6124);
  display.flushDisplay();
  display.setTextWrap(false);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &display_updater, true);
  timerAlarmWrite(timer, 4000, true);
  timerAlarmEnable(timer);
  display.fillScreen(0);
}

void loop() 
{
  static int fn=0;

  _cx=(rand()%4)*16;
  _cy=(rand()%2)*16;
  _color1=(rand()%65536); 
  dispfont16(fn++); 
  if (fn>7447) fn=0;
  
  // 送上燈板顯示
  display.showBuffer();
  
  // 延遲 20ms (50fps)
  delay(20);
}
