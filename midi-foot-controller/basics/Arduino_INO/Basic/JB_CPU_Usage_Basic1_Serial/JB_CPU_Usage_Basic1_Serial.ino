/**
 * this sample demonstrates the time needed for 3000000 loops
 * my arduino needs 7922 [ms]
 */
const unsigned long C_LOOP_COUNT = 3000000;

unsigned long _loopCounter = C_LOOP_COUNT;
unsigned long _startTime_ms;

// ----------

void setup()
{
  Serial.begin(57600);
    
  _startTime_ms = millis();
}

void loop() 
{
  if (0 == _loopCounter--) {
    // 3000000 loops in 7922 [ms]
    unsigned long endTime_ms = millis();

    Serial.print("Loops: ");
    Serial.print(C_LOOP_COUNT);
  
    Serial.print(". Time: ");
    Serial.print(endTime_ms - _startTime_ms);
    Serial.println(" [ms]");
  }
}
