#include <Arduino.h>
#include "TaskManager.hpp"

template<class DATATYPE>
class TaskCounter : public Task<DATATYPE>
{
  private:
    const char _id;
    unsigned long _counter = 0;

  public:
    TaskCounter(char id, DATATYPE interval_ms, bool running = true) 
      : Task<DATATYPE>(interval_ms, running), _id(id) {}
    
    void run(DATATYPE currentTime_ms) { _counter++; }
    virtual byte getTypeId() { return CompilerTypeId::getIdForType< TaskCounter<DATATYPE> >(); }
    
    unsigned long getCounter() { return _counter; }
    byte getId() { return _id; }
};


template<class DATATYPE>
class ArrayItemTaskCounterSum : public ArrayIterator< Task<DATATYPE> >
{
  private:
    unsigned long _sum = 0;
    
  public:
    unsigned long getSum() { return _sum; }    
    void operator() (byte index, Task<DATATYPE> &element);
};


template<class DATATYPE>
void ArrayItemTaskCounterSum<DATATYPE>::operator() (byte index, Task<DATATYPE> &element) 
{ 
  Task<DATATYPE>* pTask = ((Task<DATATYPE>*)&element);

  // if (const TaskCounter<DATATYPE>* pTaskCounter = dynamic_cast<const TaskCounter<DATATYPE>*>(&element)) {
  if (CompilerTypeId::getIdForType< TaskCounter<DATATYPE> >() == pTask->getTypeId()) {
    TaskCounter<DATATYPE>* pTaskCounter = ((TaskCounter<DATATYPE>*)&element);
    unsigned long counter = pTaskCounter->getCounter();
    _sum += counter;

    char cTemp[80];
    sprintf(cTemp, "Index: %i, ID: %c, Counter: %lu", (int)index, pTaskCounter->getId(), counter);
    Serial.println(cTemp);
  } else if (CompilerTypeId::getIdForType< TaskManager<DATATYPE> >() == pTask->getTypeId()) {
    TaskManager<DATATYPE>* pTaskManager = ((TaskManager<DATATYPE>*)&element);
    pTaskManager->forEach(*this);
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------

void setup() 
{
  Serial.begin(115200);
 
  TaskManager<unsigned int> g_taskManager_root(5, 1);

  TaskManager<unsigned int> taskManager_fast(3, 2);
  taskManager_fast.add(new TaskCounter<unsigned int>('A', 1));
  taskManager_fast.add(new TaskCounter<unsigned int>('B', 2));
  taskManager_fast.add(new TaskCounter<unsigned int>('C', 10));
  g_taskManager_root.add(&taskManager_fast);
  
  TaskManager<unsigned int> taskManager_slow(3, 100);
  taskManager_slow.add(new TaskCounter<unsigned int>('D', 10));
  taskManager_slow.add(new TaskCounter<unsigned int>('E', 500));
  taskManager_slow.add(new TaskCounter<unsigned int>('F', 25, false));
  g_taskManager_root.add(&taskManager_slow);
  
  g_taskManager_root.add(new TaskCounter<unsigned int>('G', 1));
  g_taskManager_root.add(new TaskCounter<unsigned int>('H', 2));


  unsigned long start_ms = millis();
  for (int i=0; i <= 10000; i++) {
    g_taskManager_root.updateTime(i);
  }
  unsigned long end_ms = millis();

  ArrayItemTaskCounterSum<unsigned int> forEachCounterSum;
  g_taskManager_root.forEach(forEachCounterSum);
  Serial.println(forEachCounterSum.getSum());
  Serial.println(end_ms - start_ms);
}

void loop()
{
  // put your main code here, to run repeatedly:
}