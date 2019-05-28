#ifndef COMPILERTYPEID_H
#define COMPILERTYPEID_H


#include <Arduino.h>    // ugly

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

#endif // COMPILERTYPEID_H