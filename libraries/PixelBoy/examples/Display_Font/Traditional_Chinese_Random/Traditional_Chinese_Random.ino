// PixelBoy Example: Chinese Font Test
// 

#include "wbFontB12.h"

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
uint8_t fbuf[18];
uint16_t _cx, _cy, _color1;

uint16_t textColor = display.color565(0, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
uint16_t lineColor1 = display.color565(255, 0, 0);
uint16_t lineColor2 = display.color565(255, 255, 0);
uint16_t greenColor = display.color565(0, 255, 0);

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
  display.begin(16, 18, 23, -1, 0);
  display.setFastUpdate(true);
  display.setDriverChip(FM6124);
  display.flushDisplay();
  display.setTextWrap(false);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &display_updater, true);
  timerAlarmWrite(timer, 4000, true);
  timerAlarmEnable(timer);
  display.fillScreen(myBLACK);
}

void loop() 
{
  // 清除畫面
  //display.fillScreen(myBLACK);

  _cx=(rand()%5)*13;
  _cy=(rand()%2)*14+3;
  _color1=(rand()%65536); //|0x8410;
  dispfont(rand()%5401);

  // 送上燈板顯示
  display.showBuffer();
  
  // 延遲 20ms (50fps)
  delay(20);
}
