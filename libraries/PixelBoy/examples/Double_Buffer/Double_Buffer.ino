// PixelBoy Example: Double Buffer
// 

#define double_buffer//開啟雙緩衝

#include <PxMatrix.h> //基本燈板設定
#define P_LAT 22
#define P_A 2
#define P_B 19
#define P_C 25
#define P_D 26
#define P_E 21
#define P_OE 5

hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;
PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D); //設定display物件基本數值(寬,高,OE,A,B,C,D)

void IRAM_ATTR display_updater() {
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(20);
  portEXIT_CRITICAL_ISR(&timerMux);
}

struct Text {
	char *text;
	uint16_t width, height;
	int16_t x, y;
	int16_t dx, dy;
} text1 = {"Pixel", 0, 0, 0, 0, 1, 1}, text2 = {"Boy", 0, 0, 30, 0, -1, 1};//設定飛行文字

uint16_t textColor = display.color565(0, 0, 255);     //設定基本顏色
uint16_t myBLACK = display.color565(0, 0, 0);
uint16_t lineColor1 = display.color565(255, 0, 0);
uint16_t lineColor2 = display.color565(255, 255, 0);
uint16_t greenColor = display.color565(0, 255, 0);

void setup() 
{
  display.begin(16, 18, 23, -1, 0);  //設定燈板腳位
  display.setFastUpdate(true);
  display.setDriverChip(FM6124);
  display.flushDisplay();
  display.setTextWrap(false);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &display_updater, true);
  timerAlarmWrite(timer, 4000, true);
  timerAlarmEnable(timer);
  int16_t x1 = 0, y1 = 0;
  display.getTextBounds(text1.text, 0, 0, &x1, &y1, &text1.width, &text1.height); //設定第一行文字彈跳
  display.getTextBounds(text2.text, 0, 0, &x1, &y1, &text2.width, &text2.height); //設定第二行文字彈跳
  text1.width-=2; text1.height-=2;
  text2.width-=2; text2.height-=2;
}

void loop() 
{
  static int16_t x=0, dx=1;

  // 清除畫面
  display.fillScreen(myBLACK);

  // 畫兩條線
  if(x+dx>=display.width() || x+dx<0) dx=-dx;
  x+=dx;
  display.drawLine(x,0, display.width()-x-1, display.height()-1, lineColor1);
  display.drawLine(display.width()-x-1, 0, x, display.height()-1, lineColor2);

  // 第一個字串的彈跳
  if(text1.x+text1.dx+text1.width>=display.width() || text1.x+text1.dx<0) text1.dx=-text1.dx;
  if(text1.y+text1.dy+text1.height>=display.height() || text1.y+text1.dy<0) text1.dy=-text1.dy;
  text1.x+=text1.dx;
  text1.y+=text1.dy;
  display.setTextColor(textColor);
  display.setCursor(text1.x, text1.y);
  display.print(text1.text);

  // 第二個字串的彈跳
  if(text2.x+text2.dx+text2.width>=display.width() || text2.x+text2.dx<0) text2.dx=-text2.dx;
  if(text2.y+text2.dy+text2.height>=display.height() || text2.y+text2.dy<0) text2.dy=-text2.dy;
  text2.x+=text2.dx;
  text2.y+=text2.dy;
  display.setTextColor(greenColor);
  display.setCursor(text2.x, text2.y);
  display.print(text2.text);

  // 送上燈板顯示
  display.showBuffer();
  
  // 延遲 20ms (50fps)
  delay(20);
}
