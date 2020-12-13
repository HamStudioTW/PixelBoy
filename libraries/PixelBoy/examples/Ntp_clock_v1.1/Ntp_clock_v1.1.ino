#include <pixelboy.h>
#include "wifiboy_lib.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

const char *ssid     = "Your wifi SSID";
const char *password = "Your wifi PASSWORD";
#define NTP_OFFSET   8*60*60   // GMT+8
#define NTP_INTERVAL 60*1000    
#define NTP_ADDRESS  "tw.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED);
  timeClient.begin();
  wb_init(3);
  wb_fillScreen(0);
  wb_setTextColor(wbCYAN, wbCYAN);
  wb_drawString("Connecting tw.pool.ntp.org", 0, 0, 2, 1);
  wb_drawString(ssid, 0, 15, 2, 1);
  wb_fillScreen(0);
  wb_drawString("Network Time", 5, 20, 2, 2);
  wb_setTextColor(wbYELLOW, wbYELLOW);
  wb_drawString("Taipei GMT+8", 5, 50, 2, 2);
  wb_setTextColor(wbWHITE, wbBLACK);
  init_pixelboy();
  timeClient.update();
  display.clearDisplay();
}
char timebuf[15], timestr[15];
void loop()
{
  display.clearDisplay();
  display.setTextColor(display.color565(0,255,255));
  display.setCursor(5,8);
  display.print("Pixel Boy");
  display.setCursor(6,20);
  display.print(timebuf);
  sprintf(timebuf, "%02d:%02d:%02d", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
  if (strcmp(timebuf, timestr)!=0) {
    wb_drawString(timebuf, 5, 80, 2, 2);
    strcpy(timestr, timebuf);
  }
  display.showBuffer(); 
  delay(1000); 
}
