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

// ---------- TEST using CLASS X

class X
{
  private:
    char* _pId;

  public:
    X(char* pId) : _pId(pId) {};
    char* getString() { return _pId; };
};

class ArrayItemGetString : public ArrayItem<X>
{
  void operator() (const byte index, const X& element) 
  {
    // https://www.tutorialspoint.com/c_standard_library/c_function_sprintf.htm
    char cTemp[40];
    sprintf(cTemp, "Index: %i - Value: %s", (int)index, element.getString());
    Serial.println(cTemp);
  }
} forEachGetString;


Array<X> aiX(10);

// ---------- TEST using char

class ArrayItemGetChar : public ArrayItem<char>
{
  void operator() (const byte index, const char& element) 
  {
    char cTemp[40];
    sprintf(cTemp, "Index: %i - Value: %c", (int)index, element);
    Serial.println(cTemp);
  }
} forEachGetChar;


Array<char> aiChar(2);

// ----------

void setup()
{
  Serial.begin(57600);

  aiX.add(new X("A"));
  aiX.add(new X("B"));
  aiX.forEach(forEachGetString);
  aiX.add(new X("C"));
  aiX.forEach(forEachGetString);

  Serial.println("------------");

  aiChar.add("1");
  aiChar.add("2");
  aiChar.forEach(forEachGetChar);
  aiChar.add("3");
  aiChar.forEach(forEachGetChar);
}

void loop()
{
}
