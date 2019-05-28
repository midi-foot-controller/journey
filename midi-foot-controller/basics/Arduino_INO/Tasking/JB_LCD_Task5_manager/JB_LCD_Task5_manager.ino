// loops 609067 in 10 [sec]

#include <LiquidCrystal.h>

LiquidCrystal lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode
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
  public:
    LCDStatistic(DATATYPE interval);
    void process();
};

template<typename DATATYPE>
LCDStatistic<DATATYPE>::LCDStatistic(DATATYPE interval) 
  : Task<DATATYPE>(interval)
{
}

template<typename DATATYPE>
void LCDStatistic<DATATYPE>::process()
{
  lcd.setCursor(0, 0);
  lcd.print("Loops:          ");
  lcd.setCursor(7, 0);
  lcd.print(g_loopCounter);
  g_loopCounter = 0;
}

// ----

template<typename DATATYPE>
class TaskManager
{
  private:
    Task<DATATYPE>* _pTasks[10] = { NULL };
    int _elements = 0;

  public:
    TaskManager();
    unsigned long update();

    void add(Task<DATATYPE>* pTask);
};

template<typename DATATYPE>
TaskManager<DATATYPE>::TaskManager() 
{
}

template<typename DATATYPE>
void TaskManager<DATATYPE>::add(Task<DATATYPE>* pTask)
{
  _pTasks[_elements++] = pTask;
}

template<typename DATATYPE>
unsigned long TaskManager<DATATYPE>::update()
{
  unsigned long current_ms = millis();
  // int numberOfElements = sizeof(_pTasks) / sizeof(Task<DATATYPE>*);
  for (int i = 0; i < _elements; i++) {
    _pTasks[i]->update(current_ms);
  }
  return current_ms;
}

// ----------

TaskManager<unsigned int> _taskManager;

void setup()
{
  // some displays need time to initialize
  delay(500);
  lcd.begin(16, 2);

  _taskManager.add(new LCDChar<unsigned int>('A', 'a', 1, 1000));
  _taskManager.add(new LCDChar<unsigned int>('B', 'b', 2, 2000));
  _taskManager.add(new LCDChar<unsigned int>('C', 'c', 3, 3000));
  _taskManager.add(new LCDStatistic<unsigned int>(10000));
}

void loop() 
{
  _taskManager.update();
  g_loopCounter++;
}
