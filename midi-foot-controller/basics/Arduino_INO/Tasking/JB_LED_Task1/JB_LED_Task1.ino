
const unsigned int OFFSET_ms = 10 * 1000;
unsigned long g_end_ms;
unsigned long g_loopCounter = 0;

void setup() 
{
  Serial.begin(57600);
  pinMode(LED_BUILTIN, OUTPUT);

  g_end_ms = millis() + OFFSET_ms - 1;
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);

  g_loopCounter++;
  if (millis() >= g_end_ms) {
    Serial.println(g_loopCounter);
    g_loopCounter = 0;
    g_end_ms = millis() + OFFSET_ms;
  }
}
