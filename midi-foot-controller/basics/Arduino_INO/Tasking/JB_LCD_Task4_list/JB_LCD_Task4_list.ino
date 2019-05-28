/*
  CPU Usage Test 1
*/

#include <LiquidCrystal.h>

LiquidCrystal lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode

// ----------

template<typename DATATYPE> 
class Task
{
  private:
    DATATYPE _nextTime_ms;
    DATATYPE _interval_ms;
  
  public:
    Task(DATATYPE interval_ms);
    virtual void update(DATATYPE currentTime_ms);
    
    virtual void process() = 0;
};

template<typename DATATYPE> 
Task<DATATYPE>::Task(DATATYPE interval_ms)
{  
  _interval_ms = interval_ms;
  _nextTime_ms = millis() + interval_ms;
}

template<typename DATATYPE> 
void Task<DATATYPE>::update(DATATYPE currentTime_ms)
{
  // https://playground.arduino.cc/Code/TimingRollover/
  if (static_cast<signed>(currentTime_ms - _nextTime_ms) >= 0) {
    process();
    _nextTime_ms = currentTime_ms + _interval_ms - (currentTime_ms - _nextTime_ms);
  }
}

// ----------

template<typename DATATYPE>
class LCDChar : public Task<DATATYPE>
{
  private:
    bool _toggle = false;
    char _c1, _c2, _colPos;

  public:
    LCDChar(char c1, char c2, char colPos, DATATYPE interval);
    void process();
};

template<typename DATATYPE>
LCDChar<DATATYPE>::LCDChar(char c1, char c2, char colPos, DATATYPE interval) 
  : Task<DATATYPE>(interval)
{
  _c1 = c1;
  _c2 = c2;
  _colPos = colPos;
}

template<typename DATATYPE>
void LCDChar<DATATYPE>::process()
{
  _toggle = !_toggle;
  lcd.setCursor(_colPos, 1);
  lcd.print(_toggle ? _c1 : _c2);
}



template<typename DATATYPE>
class LCDStatistic : public Task<DATATYPE>
{
  private:
    unsigned long _loopCounter = 0;
    
  public:
    LCDStatistic(DATATYPE interval);
    void update(DATATYPE currentTime_ms);
    void process();
};

template<typename DATATYPE>
LCDStatistic<DATATYPE>::LCDStatistic(DATATYPE interval) 
  : Task<DATATYPE>(interval)
{
}

template<typename DATATYPE>
void LCDStatistic<DATATYPE>::update(DATATYPE currentTime_ms)
{
  Task<DATATYPE>::update(currentTime_ms);
  _loopCounter++;
}

template<typename DATATYPE>
void LCDStatistic<DATATYPE>::process()
{
  lcd.setCursor(0, 0);
  lcd.print("Loops:          ");
  lcd.setCursor(7, 0);
  lcd.print(_loopCounter);
  _loopCounter = 0;
}

// ----

Task<unsigned int>* _task1 = new LCDChar<unsigned int>('A', 'a', 1, 1000);
Task<unsigned int>* _task2 = new LCDChar<unsigned int>('B', 'b', 2, 2000);
Task<unsigned int>* _task3 = new LCDChar<unsigned int>('C', 'c', 3, 3000);
Task<unsigned int>* _task4 = new LCDStatistic<unsigned int>(10000);
// Sketch uses 3284 Bytes (10%)

// ----------

void setup()
{
  // some displays need time to initialize
  delay(500);
  lcd.begin(16, 2);
}

void loop() 
{
  unsigned long current_ms = millis();

  // ----

  _task1->update(current_ms);
  _task2->update(current_ms);
  _task3->update(current_ms);
  _task4->update(current_ms);
}

