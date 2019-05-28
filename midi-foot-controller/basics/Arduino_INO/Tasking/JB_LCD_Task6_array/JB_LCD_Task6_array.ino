#include <LiquidCrystal.h>

LiquidCrystal g_lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode
unsigned long g_loopCounter = 0;

// ----------------------------------------------------------------------------------------------------------------------------------

template<class ELEMENT>
class ArrayItem
{
  public:
    virtual void operator() (const byte index, const ELEMENT& element) = 0;
};

template<class ELEMENT>
class Array
{
  private:
    ELEMENT* *_pElements;
    byte _elementCounter = 0;
    byte _capacity;

  public:
    Array(byte capacity);
    
    void add(ELEMENT* pElement);

    void forEach(ArrayItem<ELEMENT> &functor);
};

template<class ELEMENT>
Array<ELEMENT>::Array(byte capacity) : _capacity(capacity)
{
  _pElements = new ELEMENT*[capacity];
}

template<class ELEMENT>
void Array<ELEMENT>::add(ELEMENT* pElement)
{
  if (_elementCounter < _capacity) {
    _pElements[_elementCounter++] = pElement;
  }
}

template<class ELEMENT>
void Array<ELEMENT>::forEach(ArrayItem<ELEMENT> &functor)
{
  for (int i = 0; i < _elementCounter; i++) {
    functor((byte)i, *_pElements[i]);
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------

template<class DATATYPE> 
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

template<class DATATYPE> 
Task<DATATYPE>::Task(DATATYPE interval_ms)
{  
  _interval_ms = interval_ms;
  _nextTime_ms = millis() + interval_ms;
}

template<class DATATYPE> 
void Task<DATATYPE>::update(DATATYPE currentTime_ms)
{
  // https://playground.arduino.cc/Code/TimingRollover/
  if (static_cast<signed>(currentTime_ms - _nextTime_ms) >= 0) {
    process();
    _nextTime_ms = currentTime_ms + _interval_ms - (currentTime_ms - _nextTime_ms);
  }
}



template<class DATATYPE>
class ArrayItemTaskUpdate : public ArrayItem< Task<DATATYPE> >
{
  private:
    unsigned long _current_ms;
    
  public:
    ArrayItemTaskUpdate() {};
    
    void setCurrent_ms(const unsigned long current_ms)
    { 
      _current_ms = current_ms; 
    }
    
    void operator() (const byte index, const Task<DATATYPE> &element) 
    {
      element.update(_current_ms);
    }
};

// - - - - - 

template<typename DATATYPE>
class LCDChar : public Task<DATATYPE>
{
  private:
    bool _toggle = false;
    const char _c1, _c2, _colPos;

  public:
    LCDChar(const char c1, const char c2, const char colPos, const DATATYPE interval);
    void process();
};

template<typename DATATYPE>
LCDChar<DATATYPE>::LCDChar(const char c1, const char c2, const char colPos, const DATATYPE interval) 
  : Task<DATATYPE>(interval), _c1(c1), _c2(c2), _colPos(colPos)
{
}

template<typename DATATYPE>
void LCDChar<DATATYPE>::process()
{
  _toggle = !_toggle;
  g_lcd.setCursor(_colPos, 1);
  g_lcd.print(_toggle ? _c1 : _c2);
}

// - - - - - 

template<typename DATATYPE>
class LCDStatistic : public Task<DATATYPE>
{
  public:
    LCDStatistic(DATATYPE interval) : Task<DATATYPE>(interval) {};
    void process();
};

template<typename DATATYPE>
void LCDStatistic<DATATYPE>::process()
{
  g_lcd.setCursor(0, 0);
  g_lcd.print("Loops:          ");
  g_lcd.setCursor(7, 0);
  g_lcd.print(g_loopCounter);
  g_loopCounter = 0;
}

// ----------------------------------------------------------------------------------------------------------------------------------

template<class DATATYPE>
class TaskManager
{
  private:
    Array< Task<DATATYPE> >* _pTasks;
    ArrayItemTaskUpdate<DATATYPE> _taskUpdate;    // create one instance and reuse it.
    
  public:
    TaskManager(const byte capacity);
    void update();

    void add(Task<DATATYPE>* pTask);
};

template<class DATATYPE>
TaskManager<DATATYPE>::TaskManager(const byte capacity)
{
  _pTasks = new Array<Task<DATATYPE>>(capacity);
}

template<class DATATYPE>
void TaskManager<DATATYPE>::add(Task<DATATYPE>* pTask)
{
  _pTasks->add(pTask);
}

template<class DATATYPE>
void TaskManager<DATATYPE>::update()
{
  _taskUpdate.setCurrent_ms(millis());  // update prior calling forEach to reflect new time
  _pTasks->forEach(_taskUpdate);
}

// ----------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------

TaskManager<unsigned int> g_taskManager(20);

void setup()
{
  // some displays need time to initialize
  delay(500);
  g_lcd.begin(16, 2);

  g_taskManager.add(new LCDChar<unsigned int>('A', 'a', 1, 1000));
  g_taskManager.add(new LCDChar<unsigned int>('B', 'b', 2, 2000));
  g_taskManager.add(new LCDChar<unsigned int>('C', 'c', 3, 3000));
  g_taskManager.add(new LCDStatistic<unsigned int>(10000));
}

void loop() 
{
  g_taskManager.update();

  g_loopCounter++;
}
