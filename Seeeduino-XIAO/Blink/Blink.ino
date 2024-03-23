// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED off (HIGH is the voltage level)
  delay(50);                       // wait for a long time while the LED remains off
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED on by making the voltage LOW
  delay(50);                        // wait for a short time to briefly light the LED
}