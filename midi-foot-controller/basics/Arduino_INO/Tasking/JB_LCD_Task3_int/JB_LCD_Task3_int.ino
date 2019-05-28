#include <LiquidCrystal.h>

LiquidCrystal g_lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode

const unsigned int OFFSET_ms = 10 * 1000;
unsigned long g_endTime_ms;
unsigned long g_loopCounter = 0;

// ----------

template<typename DATATYPE> 
class Task
{
  private:
    DATATYPE _nextTime_ms;
    DATATYPE _interval_ms;
  
  public:
    Task(DATATYPE interval_ms);
    void update(DATATYPE currentTime_ms);
    
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
  g_lcd.setCursor(_colPos, 1);
  g_lcd.print(_toggle ? _c1 : _c2);
}

// ----

/*
Task<unsigned long>* _task1 = new LCDChar<unsigned long>('A', 'a', 1, 1000);
Task<unsigned long>* _task2 = new LCDChar<unsigned long>('B', 'b', 2, 2000);
Task<unsigned long>* _task3 = new LCDChar<unsigned long>('C', 'c', 3, 3000);

empty                             | 3118122
unsigned long: 3388 Bytes (11%)   | 1346800
unsigned int: 3260 Bytes (10%)    | 1038778
*/

Task<unsigned int>* _task1 = new LCDChar<unsigned int>('A', 'a', 1, 1000);
Task<unsigned int>* _task2 = new LCDChar<unsigned int>('B', 'b', 2, 2000);
Task<unsigned int>* _task3 = new LCDChar<unsigned int>('C', 'c', 3, 3000);
// Sketch uses 3284 Bytes (10%)

// ----------

void setup()
{
  // some displays need time to initialize
  delay(500);
  g_lcd.begin(16, 2);

  unsigned long current_ms = millis();
  g_endTime_ms =  + OFFSET_ms;
}

void loop() 
{
  g_loopCounter++;
  unsigned long current_ms = millis();

  // ----

  _task1->update(current_ms);
  _task2->update(current_ms);
  _task3->update(current_ms);

  // ----

  if (current_ms >= g_endTime_ms) {
    outputResults();

    g_loopCounter = 0;
    g_endTime_ms = millis() + OFFSET_ms;
  }
}


void outputResults()
{
  g_lcd.setCursor(0, 0);
  g_lcd.print("Loops:          ");
  g_lcd.setCursor(7, 0);
  g_lcd.print(g_loopCounter);
}
