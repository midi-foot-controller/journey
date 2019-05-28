template<class ELEMENT>
class ArrayIterator
{
  public:
    virtual void operator() (byte index, const ELEMENT& element) = 0;
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
    
    void add(const ELEMENT* pElement);

    void forEach(ArrayIterator<ELEMENT> &functor);
};

template<class ELEMENT>
Array<ELEMENT>::Array(byte capacity) : _capacity(capacity)
{
  _pElements = new ELEMENT*[capacity];
}

template<class ELEMENT>
void Array<ELEMENT>::add(const ELEMENT* pElement)
{
  if (_elementCounter < _capacity) {
    _pElements[_elementCounter++] = pElement;
  }
}


template<class ELEMENT>
void Array<ELEMENT>::forEach(ArrayIterator<ELEMENT> &functor)
{
  for (int i = 0; i < _elementCounter; i++) {
    functor((byte)i, *_pElements[i]);
  }
}

// ----------------------------------------------------------------------------------------------------------------------------------

class CompilerTypeId
{
  private:
    static byte generateNextId()
    {
      static byte id(0);
      return id++;
    }

  public:
    template<class T_>
    static byte getIdForType()
    {
      static byte result(generateNextId());
      return result;
    }
    
    virtual byte getTypeId() = 0;
};

// ----------------------------------------------------------------------------------------------------------------------------------

template<class DATATYPE> 
class Task : public CompilerTypeId
{
  private:
    DATATYPE _nextTime_ms;
    DATATYPE _interval_ms;
    bool _running;

  public:
    Task(DATATYPE interval_ms, byte typeId, bool running = true);
    
    virtual void updateTime(DATATYPE currentTime_ms);
    virtual void run(DATATYPE currentTime_ms) = 0;

    bool isRunning() { return _running; }
    void pause() { _running = false; }
    void start(bool immediately = true);
};

template<class DATATYPE> 
Task<DATATYPE>::Task(DATATYPE interval_ms, byte typeId, bool running) : _running(running)
{  
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



template<class DATATYPE>
class ArrayItemTaskUpdate : public ArrayIterator< Task<DATATYPE> >
{
  private:
    DATATYPE _current_ms;
    
  public:
    void setCurrent_ms(DATATYPE current_ms) { _current_ms = current_ms; }
    
    void operator() (byte index, const Task<DATATYPE> &element) 
    {
      element.updateTime(_current_ms);
    }
};

// - - - - - 

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

// ----------------------------------------------------------------------------------------------------------------------------------

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
    virtual byte getTypeId() { return CompilerTypeId::getIdForType< TaskManager<DATATYPE> >(); }

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
class ArrayItemTaskCounterSum : public ArrayIterator< Task<DATATYPE> >
{
  private:
    unsigned long _sum = 0;
    
  public:
    unsigned long getSum() { return _sum; }    
    void operator() (byte index, const Task<DATATYPE> &element);
};

template<class DATATYPE>
void ArrayItemTaskCounterSum<DATATYPE>::operator() (byte index, const Task<DATATYPE> &element) 
{ 
  Task<DATATYPE>* pTask = ((Task<DATATYPE>*)&element);

  // if (const TaskCounter<DATATYPE>* pTaskCounter = dynamic_cast<const TaskCounter<DATATYPE>*>(&element)) {
  if (CompilerTypeId::getIdForType< TaskCounter<DATATYPE> >() == pTask->getTypeId()) {
    TaskCounter<DATATYPE>* pTaskCounter = ((TaskCounter<DATATYPE>*)&element);
    unsigned long counter = pTaskCounter->getCounter();
    _sum += counter;

    char cTemp[80];
    sprintf(cTemp, "Index: %i, ID: %c, Counter: %d", (int)index, pTaskCounter->getId(), counter);
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

TaskManager<unsigned int> g_taskManager_root(5, 1);

void setup()
{
  Serial.begin(115200);

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

  for (int i=0; i <= 10000; i++) {
    g_taskManager_root.updateTime(i);
  }

  Serial.println("");
  ArrayItemTaskCounterSum<unsigned int> forEachCounterSum;
  g_taskManager_root.forEach(forEachCounterSum);
  Serial.println(forEachCounterSum.getSum());
}

void loop() 
{  
}
