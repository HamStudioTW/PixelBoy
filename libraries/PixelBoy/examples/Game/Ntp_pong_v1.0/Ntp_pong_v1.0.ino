//
// PixelBoy Example: NTP Clock & PONG Demo v1.1 (OK:ESP32)
// 
#define double_buffer
#include "pixelboy.h"
#include "wifiboy_lib.h"
#include <NTPClient.h>
#include <WiFi.h>
#define NTP_OFFSET   8*60*60   // GMT+8
#define NTP_INTERVAL 60*1000    
#define NTP_ADDRESS  "tw.pool.ntp.org"
const char *ssid     = "Your wifi SSID";
const char *password = "Your wifi PASSWORD";
char clocked=0;

void setup() 
{
  Serial.begin(115200);
  wb_init(3);
  pinMode(33,INPUT); pinMode(36,INPUT); pinMode(39,INPUT);
  pinMode(32,INPUT); pinMode(34,INPUT); pinMode(35,INPUT); 

  if (digitalRead(35)==0) {
     WiFi.begin(ssid, password);
     while (WiFi.status() != WL_CONNECTED);
     configTime(8*60*60, 0, NTP_ADDRESS);
     clocked=1;
  }
  init_pixelboy();
  wb_fillScreen(0);
  wb_drawString("Network Time", 5, 20, 2, 2);
  wb_setTextColor(wbYELLOW, wbYELLOW);
  wb_drawString("Taipei GMT+8", 5, 50, 2, 2);
  wb_setTextColor(wbWHITE, wbBLACK);
  
}

char rndstr[11][10]={
  "Great Day",
  "Hello!!!!",
  "Excellent",
  "Be Great!",
  "Wonderful",
  "You Hero!",
  "Good Game",
  "Bless You",
  "Be Nice!!",
  "Greeting!",
};

void loop() 
{
  static char timebuf[15];
  struct tm timeinfo;
  static int dx=2, dy=1, bx=20, by=0, ty=0, ry=0, rs=0;
  static uint16_t color;
  uint16_t kc[7]={0x18C0, 0x00C3, 0x1803, 0x0003, 0x00C0, 0x18C3, 0x1800};
  
  if (clocked) {
    if(!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return;
    }
    sprintf(timebuf, "%02d:%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);
  }
  
  display.fillScreen(wbBLACK); // 清除畫面

  display.drawPixel(bx, by, 0x000f);
  bx+=dx; by+=dy;
  if (bx>63) { dx=-dx; bx+=dx; }
  if (by>31 || by<0) { dy=-dy; by+=dy; }
  if (display.getPixel(bx, by)!=0) {
    dy=-dy;
  }

  ry=by-4; 
  if (ry<0) ry=0;
  if (ry>24) ry=24;
  display.fillRect(63, ry, 1, 8, 0x1042);
  
  display.fillRect(0, ty, 1, 8, 0x1082);
  if (digitalRead(33)==0) { ty--; if (ty<0) ty=0; }
  if (digitalRead(32)==0) { ty++; if (ty>24) ty=24; }

  if (bx<1) {
    if (by>ty && by<ty+8) { dx=-dx; bx+=dx; }
    else if (by==ty || by==ty+8) {
      dx=-dx; dy=-dy; bx+=dx; by+=dy; 
      color=(color+1)%7; 
      rs=abs(wb_rand(10));
      display.fillRect(5, 20, 53, 8, 0);
    }
    else bx=63;
  }
  
  display.setTextColor(kc[color]); 
  display.setCursor(5,5); 
  display.print("Pixel Boy");
  if (clocked) { 
    display.setCursor(8,20); 
    display.print(timebuf); 
  } else {
    display.setCursor(5,20); 
    display.print(rndstr[rs]); 
  }
  
  display.showBuffer(); // 送上燈板顯示
  delay(50);
}
