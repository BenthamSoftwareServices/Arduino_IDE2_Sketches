void setupIOPins(void) {
  pinMode(BUTTON_CLEAR, INPUT_PULLUP);  // set to input with internal pull up resistor
  pinMode(BUTTON_CAL, INPUT_PULLUP);    // set to input with internal pull up resistor
  pinMode(BUTTON_DEC, INPUT_PULLUP);    // set to input with internal pull up resistor
  pinMode(BUTTON_HOLD, INPUT_PULLUP);   // set to input with internal pull up resistor
  pinMode(BUTTON_BAR, INPUT_PULLUP);    // set to input with internal pull up resistor

  pinMode(analogInput, INPUT);     // used for battery status check
  pinMode(LTC2400_CS, OUTPUT);     // set for output
  digitalWrite(LTC2400_CS, HIGH);  // Setting chips select HIGH disables ADC initially
}