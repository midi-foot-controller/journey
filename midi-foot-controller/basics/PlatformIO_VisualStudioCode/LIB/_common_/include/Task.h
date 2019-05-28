#ifndef TASK_H
#define TASK_H


#include "CompilerTypeId.h"

template<class DATATYPE> 
class Task : public CompilerTypeId
{
  private:
    DATATYPE _nextTime_ms;
    DATATYPE _interval_ms;
    bool _running;
    byte _typeId;

  public:
    Task(DATATYPE interval_ms, byte typeId, bool running = true);
    
    virtual void updateTime(DATATYPE currentTime_ms);
    virtual void run(DATATYPE currentTime_ms) = 0;

    byte getTypeId() { return _typeId; }

    bool isRunning() { return _running; }
    void pause() { _running = false; }
    void start(bool immediately = true);
};


template<class DATATYPE> 
Task<DATATYPE>::Task(DATATYPE interval_ms, byte typeId, bool running)
{  
  _typeId = typeId; 
  _running = running;
  _interval_ms = interval_ms;
  _nextTime_ms = millis() + interval_ms;
}

template<class DATATYPE> 
void Task<DATATYPE>::updateTime(DATATYPE currentTime_ms)
{
  if (_running) {
    // https://playground.arduino.cc/Code/TimingRollover/
    if (static_cast<signed>(currentTime_ms - _nextTime_ms) >= 0) {
      run(currentTime_ms);
      _nextTime_ms = currentTime_ms + _interval_ms - (currentTime_ms - _nextTime_ms);
    }
  }
}

template<class DATATYPE> 
void Task<DATATYPE>::start(bool immediately)
{
  _nextTime_ms = millis();  // reset time 
  if (! immediately) {
    _nextTime_ms += _interval_ms; // no immediate start? wait for interval
  }
}

#endif // TASK_H
