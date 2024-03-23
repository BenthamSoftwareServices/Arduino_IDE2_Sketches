/*
  This example shows how to write to the STLink 2.1 Virtual Com Port, which must have Rx and Tx connected from the
  STLink 2.1 to the TX1 (A9) and RX1 (A10) respectively.
  The Arduino IDE code Serial.begin(9600); appears to configure UART1 i.e. TX1 and RX1 and ignores UARTS2 & 3.
  A diagram for connecting an STLink 2.1 to a "Blue Pill" module is at the following location:
  E:\Cloud Storage\Dropbox\Personal\Electronics\MCUs & Peripherals\ARM\STM Microelectronics\ST-Link debugger hardware\V2.1\ST-Link to target wiring.tif
  
  Don't confuse Serial.write() with Serial.print()!
  Serial.write() writes binary data to the serial port.
  This data is sent as a byte or series of bytes; to send the characters representing the digits of a number use the print() function instead.
*/
int x;

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  for (int x = 0; x < 64; x++) {  // only part of the ASCII chart, change to suit
    // print it out in many formats:
    Serial.print(x);       // print as an ASCII-encoded decimal - same as "DEC"
    Serial.print("\t\t");  // prints two tabs to accomodate the label lenght

    Serial.print(x, DEC);  // print as an ASCII-encoded decimal
    Serial.print("\t");    // prints a tab

    Serial.print(x, HEX);  // print as an ASCII-encoded hexadecimal
    Serial.print("\t");    // prints a tab

    Serial.print(x, OCT);  // print as an ASCII-encoded octal
    Serial.print("\t");    // prints a tab

    Serial.println(x, BIN);  // print as an ASCII-encoded binary
    // then adds the carriage return with "println"

    digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on by making the voltage LOW
    delay(100);                       // wait for a second
  }
  Serial.println();  // prints another carriage return
}
