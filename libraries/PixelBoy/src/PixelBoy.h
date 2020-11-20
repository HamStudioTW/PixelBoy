#ifndef _PIXELBOY_H_
#define _PIXELBOY_H_
//
// PixelBoy for OK:ESP32 
// with PxMatrix Library (by Dominic Buchstaller)
//

#define wbBLACK       0x0000      /*   0,   0,   0 */
#define wbNAVY        0x0007      /*   0,   0, 128 */
#define wbDARKGREEN   0x01E0      /*   0, 128,   0 */
#define wbDARKCYAN    0x01E7      /*   0, 128, 128 */
#define wbMAROON      0x1800      /* 128,   0,   0 */
#define wbPURPLE      0x780F      /* 128,   0, 128 */
#define wbOLIVE       0x7BE0      /* 128, 128,   0 */
#define wbLIGHTGREY   0xC618      /* 192, 192, 192 */
#define wbDARKGREY    0x7BEF      /* 128, 128, 128 */
#define wbBLUE        0x000F     /*   0,   0, 255 */
#define wbGREEN       0x07E0      /*   0, 255,   0 */
#define wbCYAN        0x01E7      /*   0, 255, 255 */
#define wbRED         0x3800      /* 255,   0,   0 */
#define wbMAGENTA     0x780F      /* 255,   0, 255 */
#define wbYELLOW      0x79E0      /* 255, 255,   0 */
#define wbWHITE       0x39EF      /* 255, 255, 255 */
#define wbORANGE      0xFD20      /* 255, 165,   0 */
#define wbGREENYELLOW 0xAFE5      /* 173, 255,  47 */
#define wbPINK        0x780F

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

void init_pixelboy()
{
  display.begin(16, 18, 23, -1, 0);
  display.setFastUpdate(false);
  display.setDriverChip(FM6124);
  display.flushDisplay();
  display.setTextWrap(false);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &display_updater, true);
  timerAlarmWrite(timer, 4000, true);
  timerAlarmEnable(timer);
}
  #endif //  _PIXELBOY_H__