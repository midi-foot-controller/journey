#ifndef LCDCHAR_H
#define LCDCHAR_H


#include <LiquidCrystal.h>

#include <Task.h>

class LcdChar : public Task<unsigned int>
{
  private:
    LiquidCrystal* _pLCD; 
    bool _toggle = false;
    char _c1, _c2, _colPos;

  public:
    LcdChar(LiquidCrystal* pLCD, char c1, char c2, char colPos, unsigned int interval_ms);
    void run(unsigned int currentTime_ms);
};

LcdChar::LcdChar(LiquidCrystal* pLCD, char c1, char c2, char colPos, unsigned int interval_ms) 
  : Task(interval_ms, CompilerTypeId::getIdForType< LcdChar>())
{
  _pLCD = pLCD;
  _c1 = c1;
  _c2 = c2;
  _colPos = colPos;
}

void LcdChar::run(unsigned int currentTime_ms)
{
  _toggle = !_toggle;
  _pLCD->setCursor(_colPos, 0);
  _pLCD->print(_toggle ? _c1 : _c2);
}

#endif // LCDCHAR_H