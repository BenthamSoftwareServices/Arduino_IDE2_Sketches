/*
 * Blink Built-in LED on Digital Pin 9
 * Using AVR Dragon programmer board for Arduino Pro Mini boards 
 */
#define LED_Yellow 9 // Yellow lED on digital pin 9 = PortB Pin1

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_Yellow, OUTPUT);
}

void loop() {
  digitalWrite(LED_Yellow, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(LED_Yellow, LOW);   // turn the LED off by making the voltage LOW
  delay(100);                      // wait for a second
}
