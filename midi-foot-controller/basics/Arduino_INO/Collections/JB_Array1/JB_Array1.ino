template<class T>
class Array
{
  private:
    T* *_pElements;
    byte _elementCounter = 0;

  public:
    Array(byte capacity);
    void add(T* pElement);
};

template<class T>
Array<T>::Array(byte capacity)
{
  _pElements = new T*[capacity];
}

template<class T>
void Array<T>::add(T* pElement)
{
  _pElements[_elementCounter++] = pElement;
}

// ----------

void setup()
{
  Array<char> aiChar(5);
  aiChar.add("1");
  aiChar.add("2");
}

void loop()
{
}


