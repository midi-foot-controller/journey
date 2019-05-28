/*
  CPU Usage Test 1
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode

// unsigned long can keep values from 0 to 4294967295 (2^32 - 1)
// https://www.arduino.cc/reference/en/language/variables/data-types/unsignedlong/
unsigned long _endTime_me;
unsigned long _loopCounter = 0;

unsigned long _nextMeasurement_ms;
unsigned long _cpuLoopCounter = 0;
unsigned int _avgCpuUsage_perc = 0;

boolean _cpuBlock = true;

boolean _done = false;

const int c_100 = 100;
const int c_1000_ms = 1000;
const unsigned long c_MaxLoopCount = 168000;

// ----------

void setup()
{
  // some displays need time to initialize
  delay(1000);
  // initialize LCD and set up the number of columns and rows
  lcd.begin(16, 2);
  // testing time 10 seconds = 10 * c_1000_ms [ms] = 10000 [ms]
  _endTime_me = millis() + 10 * c_1000_ms;
  
  _nextMeasurement_ms = millis() + c_1000_ms;
}

void loop() 
{
  if (! _done) {
    // be careful: millis() is expensive and eats up 21% of CPU time.
    unsigned long currentTime_ms = millis();
    
    if (_cpuBlock) {
      delay(765);
      _cpuBlock = false;
    }
    
    if (_nextMeasurement_ms < currentTime_ms) {      
      if (_cpuLoopCounter > c_MaxLoopCount) {
        _cpuLoopCounter = c_MaxLoopCount;
      }

      int usage_perc = c_100 - (_cpuLoopCounter * c_100 / c_MaxLoopCount);
      if (usage_perc > c_100) {
        usage_perc = c_100;
      }
      _avgCpuUsage_perc = (usage_perc + _avgCpuUsage_perc) / 2;
      _nextMeasurement_ms = currentTime_ms + c_1000_ms;
      _cpuLoopCounter = 0;

      _cpuBlock = true;
    }
    
    if (_endTime_me < currentTime_ms) {
      // the loop counter is 2891488, means (aproximately) we ran 3 mio times through our loop in 10 seconds 
      // beside incrementing we have 1 call to millis() and two compare functions in each loop, honestly we stole some processing power
      //
      //     460 loops in     1 [ms] = 460 [loops / ms] ???
      //  289148 loops in  1000 [ms] = 289 [loops / ms]
      // 2891488 loops in 10000 [ms] = 289 [loops / ms]
      // --> 289000 [loops / sec]
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Loop:");
      lcd.setCursor(6, 0);
      lcd.print(_loopCounter / 10);

      lcd.setCursor(0, 1);
      lcd.print("CPU:");
      lcd.setCursor(6, 1);
      // prints 98% instead of 100%, becasue we start with 0% and calculate an average between old and new, thus the value increases towards 100% over time
      lcd.print(_avgCpuUsage_perc);
      lcd.print("%");

      _done = true;
    } else {
      _loopCounter++;
      _cpuLoopCounter++;
    }
  }
}
