static int potPin = A7; // Pot that can vary between 0-5v connected to Analog 7 pin.  Declared as static so it can't be changed.
int pot = 512;          // pot holds the result of the analogRead of the voltage on A7, ranges from 0-1023 i.e. a 10 bit binary number
int mapped = 127;       // mapped holds the result of mapping pot over a smaller range to convert to an 8 bit binary number suitable for a PWM analogWrite
static int PWMpin = 3;  // Declare PWMpin to be digital pin 3. Declared as static so it can't be changed.

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);

  TCCR2A = TCCR2A & 0b11111100;  // clear lower 2 bits of TCCR2A to zeros
  TCCR2A = TCCR2A | 0b00000001;  // sets lower 2 bits of TCCR2A to 01
  TCCR2B = TCCR2B & 0b11110000;  // clear lower 4 bits of TCCR2B to zeros
  TCCR2B = TCCR2B | 0b00001011;  // sets lower 4 bits of TCCR2B to 1011
                                 // Configure Timer/Counter 2 to waveform generator mode 5 i.e. PWM, Phase Correct and with no prescaler
  OCR2A = 0xF8;                  // OCR2A sets the top value for the PWM counter in mode 5. E8 = 1.08KHz, F8 = 1.01KHz, FF = 977Hz

  Serial.print("\x1B[1;1H\x1B[2J");  // ANSI VT100 Escape sequences to move terminal cursor to position 1,1 then clear the terminal window

  Serial.print("\x1b[1;31m");  // Set terminal style to bold, red foreground.  See https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

  delay(1000); //Blank terminal windows showing before next printing sequence to verify changes to control registers

  Serial.print("TCCR2A = ");    // Print text
  Serial.println(TCCR2A, BIN);  // Print value of TCCR2A register in binary

  Serial.print("TCCR2B = ");    // Print text
  Serial.println(TCCR2B, BIN);  // Print value of TCCR2B register in binary

  Serial.print("OCR2A = ");     // Print text
  Serial.println(OCR2A, HEX);   // Print value of OCR2A register in hex
  Serial.println("=========");  // Print text
}

void loop() {
  // put your main code here, to run repeatedly:
  pot = analogRead(potPin);            // read the analog voltage on potPin and convert to a value between 0 and 1023
  mapped = map(pot, 0, 1023, 1, 247);  // map a 10-bit number to an 8-bit number as a PWM analogWrite only uses 8-bits.  See: https://www.arduino.cc/reference/en/language/functions/math/map/
                                       // Mapping to 0,255 causes PWM output to be static at 0v or 5v.  Mapping to 1,247 produces PWM pulses at the extremes of the Potentiometer movement and 247 is
                                       // used rather than 254 as OCR2A is set to 0xF8 so Timer/Counter 2 count up stops and reverses at 0xF8, not 0xFF as it does in modes 0.1 & 3. See page 205 of
                                       // ATmega328P full data sheet.
  analogWrite(PWMpin, mapped);         // write the value of mapped to digital pin 3 (PWMpin) to set PWM
  delay(500);
  //Serial.println(pot);
}
