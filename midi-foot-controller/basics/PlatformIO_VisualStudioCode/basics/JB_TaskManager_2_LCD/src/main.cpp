// https://docs.platformio.org/en/latest/librarymanager/index.html
// https://platformio.org/lib/show/887/LiquidCrystal/installation

#include <Arduino.h>
#include <LiquidCrystal.h>

#include <TaskManager.h>
#include <LcdChar.h>
#include <LcdLoopCounter.h>


LiquidCrystal g_lcd(14, 12,   4, 5, 6, 7,   8, 9, 10, 11);  // 8 bit mode
TaskManager<unsigned int> g_taskManager_root(5, 1);

unsigned long g_loopCounter = 0;


void setup() 
{
  Serial.begin(115200);
  g_lcd.begin(16, 2);
 
  g_taskManager_root.add(new LcdChar(&g_lcd, 'A', 'a', 1, 1000));
  g_taskManager_root.add(new LcdChar(&g_lcd, 'B', 'b', 2, 2000));
  g_taskManager_root.add(new LcdChar(&g_lcd, 'C', 'c', 3, 3000));
  g_taskManager_root.add(new LcdLoopCounter(&g_lcd));
}

void loop()
{
  g_taskManager_root.updateTime(millis());

  g_loopCounter++;
}