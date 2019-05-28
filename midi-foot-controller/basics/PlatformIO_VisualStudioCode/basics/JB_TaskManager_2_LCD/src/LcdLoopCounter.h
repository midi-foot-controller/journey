#ifndef LCDLOOPCOUNTER_H
#define LCDLOOPCOUNTER_H


#include <LiquidCrystal.h>

#include <Task.h>

extern unsigned long g_loopCounter;   // be careful

class LcdLoopCounter : public Task<unsigned int>
{
  private:
    LiquidCrystal* _pLCD;

  public:
    LcdLoopCounter(LiquidCrystal* pLCD);
    void run(unsigned int currentTime_ms);
};

LcdLoopCounter::LcdLoopCounter(LiquidCrystal* pLCD) 
  : Task(10000, CompilerTypeId::getIdForType<LcdLoopCounter>())
{
  _pLCD = pLCD;
}

void LcdLoopCounter::run(unsigned int currentTime_ms)
{
  _pLCD->setCursor(0, 1);
  _pLCD->print("Loops:          ");
  _pLCD->setCursor(7, 1);
  _pLCD->print(g_loopCounter);
  
  g_loopCounter = 0;
}

#endif // LCDLOOPCOUNTER_H