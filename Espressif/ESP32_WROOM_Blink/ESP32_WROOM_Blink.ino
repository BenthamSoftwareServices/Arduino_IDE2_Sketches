/*
  Blink Built-in LED on Digital Pin 2
*/
#define LED 2

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_Yellow, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED, LOW);   // turn the LED off by making the voltage LOW
  delay(100);                      // wait for a second
}
