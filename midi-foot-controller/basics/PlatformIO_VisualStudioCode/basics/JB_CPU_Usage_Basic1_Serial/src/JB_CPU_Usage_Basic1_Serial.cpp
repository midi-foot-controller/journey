const unsigned long C_LOOP_COUNT = 3000000;
unsigned long _startTime_ms;
unsigned long _loopCounter = C_LOOP_COUNT;

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  _startTime_ms = millis();
}

void loop() 
{
  if (0 == _loopCounter--) {
    // 3000000 loops in 6225 [ms]
    unsigned long endTime_ms = millis();
    float total_ms = ((float)(endTime_ms - _startTime_ms));
    int numberOfDigits = log10(total_ms) + 1;
    for (int i = 0; i <= numberOfDigits; i++) {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(2000);
      digitalWrite(LED_BUILTIN, LOW);
      delay(250);

      int blinkCount = (int)total_ms;
      for (int b = 0; b < blinkCount; b++) {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
        delay(250);
      }

      delay(1000);

      total_ms = (total_ms - blinkCount) * 10;
    }
  }
}
