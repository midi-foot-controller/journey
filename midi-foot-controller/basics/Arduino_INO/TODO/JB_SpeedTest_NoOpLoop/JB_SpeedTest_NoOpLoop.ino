/*
  New LCD Library (Speed Test)
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode

int _version = 1;

unsigned long _endTime_me;
unsigned long _loopCounter = 0;
boolean _done = false;
char _row1[] = "*FirstSpeedTest*";
char _row2[] = "?FirstSpeedTest?";

// ----------

void setup()
{
  // some displays need time to initialize
  delay(1000);
  // initialize LCD and set up the number of columns and rows
  lcd.begin(16, 2);
  // testing time 10 seconds = 10 * 1000 [ms] = 10000 [ms]
  _endTime_me = millis() + 10 * 1000;
}

void loop() 
{
  if (! _done) {
    if (_endTime_me < millis()) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loop Counter:");
      lcd.setCursor(0, 1);
      lcd.print(_loopCounter);
      _done = true;
    } else {
      // our code will always check all versions otherwise the number of "if" conditions is not the same between tests
      
      if (1 == _version) {
        // ORIG: 1710 --> 171 loops per sec = 171 [loops / sec] or "frames per sec"
        // NEW:  4141 --> 414 [loops / sec] ... (2.4 times faster)
        // 
        // 414 frames = 100% computing time 
        // --> display time for one frame = 1000 [ms] / 414 frames = 2.415 [ms / frame]
        // --> fixed 25 Hz refresh rate = 25 * 2.415 [ms] = 60.38 [ms] or 16.7% of total available computing time
        lcd.setCursor(0, 0);
        lcd.print(_row1);
        lcd.setCursor(0, 1);
        lcd.print(_row2);
      }

      if (2 == _version) {    
        // ORIG:  862 -->  86 frames per sec
        // NEW:  2048 --> 204 frames per sec (2.4 times faster)
        // compiler adds a trailing "0" to our string
        for (int x = 0; x < sizeof(_row1); x++) {
          lcd.setCursor(x, 0);
          lcd.print(_row1[x]);
        }

        for (int x = 0; x < sizeof(_row2); x++) {
          // ORIG:  6441 -->  644 frmaes per sec --> div by 4 = 161 (almost linear to _version == 1, beside some missing setCursor calls)
          // NEW:  15486 --> 1548 frmaes per sec (2.4 times faster) --> div by 4 = 387 (almost linear to _version == 1, beside some missing setCursor calls)
          lcd.setCursor(x, 1);  
          lcd.print(_row2[x]);
        }
      }

      if (3 == _version) {
          lcd.setCursor(0, 0);
          lcd.print("only 8..");
      }

      _loopCounter++;
    }
  }
}
