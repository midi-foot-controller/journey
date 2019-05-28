#ifndef ARRAY_H
#define ARRAY_H


#include <Arduino.h>    // ugly

template<class ELEMENT>
class ArrayIterator
{
  public:
    virtual void operator() (byte index, ELEMENT& element) = 0;
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

    void forEach(ArrayIterator<ELEMENT> &functor);
};

// ----

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
void Array<ELEMENT>::forEach(ArrayIterator<ELEMENT> &functor)
{
  for (int i = 0; i < _elementCounter; i++) {
    functor((byte)i, *_pElements[i]);
  }
}

#endif // ARRAY_H