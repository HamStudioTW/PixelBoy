
#ifndef PatternTest_H
#define PatternTest_H

class PatternTest : public Drawable {
  private:

  public:
    PatternTest() {
      name = (char *)"Test Pattern";
    }

    unsigned int drawFrame() {

       display.fillScreen(display.color565(255, 0, 0));
       delay(0.5);
       display.fillScreen(display.color565(0, 255, 0));
       delay(0.5);
       display.fillScreen(display.color565(0, 0, 255));
       delay(0.5);  
    
    }
};

#endif
