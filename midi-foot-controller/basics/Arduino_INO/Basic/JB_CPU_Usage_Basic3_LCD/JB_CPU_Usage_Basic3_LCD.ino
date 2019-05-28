/*
  CPU Usage Test 1
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode

const unsigned long C_LOOP_COUNT = 3000000;
unsigned long _startTime_ms;

unsigned long _loopCounter = C_LOOP_COUNT;

// ----------

void setup()
{
  _startTime_ms = millis();
}

void loop() 
{
  if (0 == _loopCounter--) {
    // 3000000 loops in 6225 [ms]
    unsigned long endTime_ms = millis();
  
    // some displays need time to initialize
    delay(1000);
    // initialize LCD and set up the number of columns and rows
    lcd.begin(16, 2);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Loops: ");
    lcd.print(C_LOOP_COUNT);
  
    lcd.setCursor(0, 1);
    lcd.print("Time: ");
    lcd.print(endTime_ms - _startTime_ms);
    lcd.print("[ms]");
  }
}
