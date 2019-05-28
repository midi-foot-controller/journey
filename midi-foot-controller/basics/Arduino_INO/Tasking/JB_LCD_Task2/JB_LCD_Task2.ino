/*
  CPU Usage Test 1
*/

#include <LiquidCrystal.h>

LiquidCrystal g_lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode

const unsigned int OFFSET_ms = 10 * 1000;
unsigned long g_endTime_ms;
unsigned long g_loopCounter = 0;

// ----------

class Task
{
  private:
    unsigned long _nextTime_ms;
    unsigned long _interval_ms;
  
  public:
    Task(unsigned long interval_ms);
    void update(unsigned long currentTime_ms);
    
    virtual void process() = 0;
};

Task::Task(unsigned long interval_ms)
{
  _interval_ms = interval_ms;
  _nextTime_ms = millis() + interval_ms;
}

void Task::update(unsigned long currentTime_ms)
{
  if (currentTime_ms >= _nextTime_ms) {
    process();
    _nextTime_ms = currentTime_ms + _interval_ms - (currentTime_ms - _nextTime_ms);
  }
}

// ----------

class g_lcdChar : public Task
{
  private:
    bool _toggle = false;
    char _c1, _c2, _colPos;

  public:
    g_lcdChar(char c1, char c2, char colPos, unsigned long interval);

    void process();
};

g_lcdChar::g_lcdChar(char c1, char c2, char colPos, unsigned long interval) 
  : Task(interval)
{
  _c1 = c1;
  _c2 = c2;
  _colPos = colPos;
}

void g_lcdChar::process()
{
  _toggle = !_toggle;
  g_lcd.setCursor(_colPos, 1);
  g_lcd.print(_toggle ? _c1 : _c2);
}

// ----

Task* _task1 = new g_lcdChar('A', 'a', 1, 1000);
Task* _task2 = new g_lcdChar('B', 'b', 2, 2000);
Task* _task3 = new g_lcdChar('C', 'c', 3, 3000);

// ----------

void setup()
{
  // some displays need time to initialize
  delay(500);
  // initialize g_lcd and set up the number of columns and rows
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

// 3117802 - 3118122 | ca. 1347292

void outputResults()
{
  g_lcd.setCursor(0, 0);
  g_lcd.print("Loops:          ");
  g_lcd.setCursor(7, 0);
  g_lcd.print(g_loopCounter);
}
