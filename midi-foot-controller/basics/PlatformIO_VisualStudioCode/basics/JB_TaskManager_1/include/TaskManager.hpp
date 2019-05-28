#ifndef TASKMANAGER_H
#define TASKMANAGER_H


#include "Array.hpp"
#include "CompilerTypeId.hpp"
#include "Task.hpp"

template<class DATATYPE>
class ArrayItemTaskUpdate : public ArrayIterator< Task<DATATYPE> >
{
  private:
    DATATYPE _current_ms;
    
  public:
    void setCurrent_ms(DATATYPE current_ms) { _current_ms = current_ms; }
    
    void operator() (byte index, Task<DATATYPE> &element) 
    {
      element.updateTime(_current_ms);
    }
};


template<class DATATYPE>
class TaskManager : public Task<DATATYPE>
{
  private:
    Array< Task<DATATYPE> >* _pTasks;
    ArrayItemTaskUpdate<DATATYPE> _taskUpdate;    // create one instance and reuse it.

  public:
    TaskManager(byte capacity, DATATYPE interval_ms = 1, bool running = true);

    void add(Task<DATATYPE>* pTask);
    virtual void run(DATATYPE currentTime_ms);
    virtual byte getTypeId();

    void forEach(ArrayIterator< Task<DATATYPE> > &functor);
};


template<class DATATYPE>
TaskManager<DATATYPE>::TaskManager(byte capacity, DATATYPE interval_ms, bool running) 
  : Task<DATATYPE>(interval_ms, running)
{
  _pTasks = new Array<Task<DATATYPE>>(capacity);
}

template<class DATATYPE>
void TaskManager<DATATYPE>::add(Task<DATATYPE>* pTask)
{
  _pTasks->add(pTask);
}

template<class DATATYPE>
void TaskManager<DATATYPE>::run(DATATYPE currentTime_ms)
{
  _taskUpdate.setCurrent_ms(currentTime_ms);  // update prior calling forEach to reflect new time
  forEach(_taskUpdate);
}

template<class DATATYPE>
void TaskManager<DATATYPE>::forEach(ArrayIterator< Task<DATATYPE> > &functor)
{
  _pTasks->forEach(functor);
}

template<class DATATYPE>
byte TaskManager<DATATYPE>::getTypeId() 
{ 
  return CompilerTypeId::getIdForType< TaskManager<DATATYPE> >(); 
}

#endif // TASKMANAGER_H