// Pixelboy Example: Aurora Demo
// Nov 11, 2020 Ham Studio
/*
基於各種數學函數所產生的各式特效
可於Pattern.h修改顯示的特效
PATTERN_COUNT = ; 後面的數字請一定要改成對應特效數量
*/

#define PxMATRIX_double_buffer true

#include <PxMatrix.h>
#include <pixelboy.h>
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 32

unsigned long ms_current  = 0;
unsigned long ms_previous = 0;
unsigned long ms_animation_max_duration = 10000; // 10秒換一個展示動畫
unsigned long next_frame = 0;


#include <FastLED.h>
#include "Effects.h"
Effects effects;
#include "Drawable.h"
#include "Playlist.h"
#include "Patterns.h"
Patterns patterns;

uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};

void display_update_enable(bool is_enable)
{
  if (is_enable) {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  } else {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

unsigned long last_draw=0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    uint16_t text_length = text.length();
    display.setTextWrap(false);  
    display.setTextSize(1);
    display.setRotation(0);
    display.setTextColor(display.color565(colorR,colorG,colorB));
    for (int xpos=MATRIX_WIDTH; xpos>-(MATRIX_WIDTH+text_length*5); xpos--)
    {
      display.setTextColor(display.color565(colorR,colorG,colorB));
      display.clearDisplay();
      display.setCursor(xpos,ypos);
      display.println(text);
      delay(scroll_delay);
      yield();
    }
}

void setup() 
{
  Serial.begin(115200);
  
  display.begin(16, 18, 23, -1, 0);
  display.setFastUpdate(true);
  display.setDriverChip(FM6124);
  display.setPanelsWidth(1);
  display.clearDisplay();
  display.setTextColor(myCYAN);
  display.setCursor(2,5);
  display.print("PixelBoy");
  display.setTextColor(myMAGENTA);
  display.setCursor(2,15);
  display.print("Play to learn");
  display_update_enable(true);
  delay(5000);
  
  scroll_text(10, 60, "Hello, PixelBoy! Enjoy!", 255, 0, 0);

  effects.Setup();
  delay(500);
  Serial.println("Effects being loaded: ");
  patterns.setPattern(0); 
  patterns.start();     
  Serial.print("Starting with pattern: ");
  Serial.println(patterns.getCurrentPatternName());
}

void loop()
{
    ms_current = millis();
    if ((ms_current - ms_previous) > ms_animation_max_duration) {
       patterns.stop();      
       patterns.move(1);
       patterns.start();  
       Serial.print("Changing pattern to:  ");
       Serial.println(patterns.getCurrentPatternName());
       ms_previous = ms_current;
    }
    if (next_frame < ms_current) {
      next_frame = patterns.drawFrame() + ms_current;   
      display.showBuffer();  
    }
}
