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

// ----

class X : public CompilerTypeId
{
  public:
    byte _id;
    X(byte id) : _id(id) {}
    virtual byte getTypeId() { return CompilerTypeId::getIdForType<X>(); }
};

class Y : public CompilerTypeId
{
  public:
    byte _id;
    Y(byte id) : _id(id) {};
    virtual byte getTypeId() { return CompilerTypeId::getIdForType<Y>(); }
};

class YD : public Y
{
  public:
    YD(byte id) : Y(id) {};
    virtual byte getTypeId() { return CompilerTypeId::getIdForType<YD>(); }
};

void setup()
{
  Serial.begin(115200);

  X x1 = X(1);
  X x2 = X(2);
  Y y1 = Y(3);
  Y y2 = Y(4);
  YD yd1 = YD(5);

  Serial.print("X: id="); Serial.print(x1._id); Serial.print(" type="); Serial.println(x1.getTypeId());
  Serial.print("X: id="); Serial.print(x2._id); Serial.print(" type="); Serial.println(x2.getTypeId());
  Serial.print("Y: id="); Serial.print(y1._id); Serial.print(" type="); Serial.println(y1.getTypeId());
  Serial.print("Y: id="); Serial.print(y2._id); Serial.print(" type="); Serial.println(y2.getTypeId());
  Serial.print("YD: id="); Serial.print(yd1._id); Serial.print(" type="); Serial.println(yd1.getTypeId());
  Serial.print("X: type="); Serial.println(CompilerTypeId::getIdForType<X>());  
  Serial.print("Y: type="); Serial.println(CompilerTypeId::getIdForType<Y>());  
  Serial.print("YD: type="); Serial.println(CompilerTypeId::getIdForType<YD>());  
}

void loop()
{
}
