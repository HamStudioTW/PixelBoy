#include <PxMatrix.h>

#include "wifiboy_lib.h"
#include <NTPClient.h>
#include <WiFi.h>
#include <WiFiUdp.h>

#define NTP_OFFSET   8*60*60   // GMT+8
#define NTP_INTERVAL 60*1000    
#define NTP_ADDRESS  "tw.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);

const char *ssid     = "你的WiFi名稱";
const char *password = "你的WiFi密碼";

// Pins for LED MATRIX
#ifdef ESP32

#define P_LAT 22
#define P_A 2
#define P_B 19
#define P_C 25
#define P_D 26
#define P_E 21
#define P_OE 5
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#endif

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 32

unsigned long ms_current  = 0;
unsigned long ms_previous = 0;
unsigned long ms_animation_max_duration = 10000; // 10 seconds
unsigned long next_frame = 0;

uint8_t display_draw_time=30; //30-60 is usually fine

//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
//PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
PxMATRIX display(64,32,P_LAT,P_OE,P_A,P_B,P_C,P_D); //,P_D,P_E);

#include <FastLED.h> // Aurora needs fastled

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t myCOLORS[8]={myRED,myGREEN,myBLUE,myWHITE,myYELLOW,myCYAN,myMAGENTA,myBLACK};

#ifdef ESP32
void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}
#endif

int A=0;

void display_update_enable(bool is_enable)
{

#ifdef ESP32
  if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  }
  else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
#endif
}


void setup() {

 Serial.begin(115200);
  // Define your display layout here, e.g. 1/8 step, and optional SPI pins begin(row_pattern, CLK, MOSI, MISO, SS)
  display.begin(16, 18, 23, -1, -1);
  display.setFastUpdate(true);
  //display.begin(8, 14, 13, 12, 4);
  display.setDriverChip(FM6124); //FM6126A); //FM6126A); // FM6124
  //display.setBrightness(200);
  wb_init(3);
  wb_fillScreen(0);
  wb_setTextColor(wbCYAN, wbCYAN);
  wb_drawString("Connecting tw.pool.ntp.org", 0, 0, 2, 1);
  wb_drawString(ssid, 0, 15, 2, 1);
  
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED);
  
  timeClient.begin();

  wb_fillScreen(0);
  wb_drawString("Network Time", 5, 20, 2, 2);
  wb_setTextColor(wbYELLOW, wbYELLOW);
  wb_drawString("Taipei GMT+8", 5, 50, 2, 2);
  wb_setTextColor(wbWHITE, wbBLACK);


  //display.setFastUpdate(true);
  display.clearDisplay();
  display.setTextColor(myCYAN);
  display.setCursor(2,5);
  display.print("WiFiBoy");
  display.setTextColor(myMAGENTA);
  display.setCursor(2,15);
  display.print("PixelFun");
  display_update_enable(true);
}

unsigned long last_draw=0;
void scroll_text(uint8_t ypos, unsigned long scroll_delay, String text, uint8_t colorR, uint8_t colorG, uint8_t colorB)
{
    uint16_t text_length = text.length();
    display.setTextWrap(false);  // we don't wrap text so it scrolls nicely
    display.setTextSize(1);
    display.setRotation(0);
    display.setTextColor(display.color565(colorR,colorG,colorB));

    // Asuming 5 pixel average character width
    for (int xpos=MATRIX_WIDTH; xpos>-(MATRIX_WIDTH+text_length*5); xpos--)
    {
      display.setTextColor(display.color565(colorR,colorG,colorB));
      display.clearDisplay();
      display.setCursor(xpos,ypos);
      display.println(text);
      delay(scroll_delay);
      yield();
      delay(scroll_delay/5);
      yield();

    }
}

char timebuf[15], timestr[15];
void loop()
{
  static int n = 0;
  timeClient.update();
  display.clearDisplay();
  for(int i=0; i<64; i++) {
      display.drawPixelRGB888(i, 0, 50,0,0);
      display.drawPixelRGB888(i, 31, 0,50,0);
  }
  for(int j=0; j<32; j++) {
      display.drawPixelRGB888(0, j, 0,0,50);
      display.drawPixelRGB888(63, j, 50,0,50);
  }


  display.setTextColor(myCYAN);
  display.setCursor(8,5);
  display.print("PiexlBoy");
  display.setTextColor(myMAGENTA);
  display.setCursor(8,15);
  display.print(timebuf);

  sprintf(timebuf, "%02d:%02d:%02d", timeClient.getHours(), timeClient.getMinutes(), timeClient.getSeconds());
  if (strcmp(timebuf, timestr)!=0) {
    wb_drawString(timebuf, 5, 80, 2, 2);
    strcpy(timestr, timebuf);
  }
  
  display.showBuffer(); 
  delay(1000); 
  n++;
}
