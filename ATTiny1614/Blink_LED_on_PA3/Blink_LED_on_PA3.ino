// Blink LEDs on PA3 an ATTiny1614 MCU
// The MCU clock speed can be configured via TOOLS > CLOCK > xMHz, faster reduces the loop execution time but has no effect on the millis function.

// MCU pin definitions
// Important - always use the full pin definition i.e. PXn where X is the port letter and n the pin number

const int ledPinBlue = PIN_PA3; // Pin 3 Port A


// Variable definitions
unsigned int A = 888; //dividend
unsigned int B = 444; //divisor

void setDefaultPinStates() {
  pinMode(ledPinBlue, OUTPUT); //set as output
  }

void setup() {
  setDefaultPinStates(); //run function
  delay (1000);
}

void loop() {
/*
  Flash the LEDs on PA3 with a single line of code.
  The remainder of millis divided by A is compared to B
  and when greater the result is 1 and when less is 0, this binary result
  is written to the appropriate ledPin
*/
  digitalWrite(ledPinBlue, millis() % A > B);
}