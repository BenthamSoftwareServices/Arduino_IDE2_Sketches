// Blink LEDs on an ATTiny1614 MCU, also includes code to configure and use the watchdog timer to reset the MCU in case of failure.
// The MCU clock speed can be configured via TOOLS > CLOCK > xMHz, faster reduces the loop execution time but has no effect on the millis function.

//#include <avr/wdt.h> // Include the watchdog timer library

// MCU pin definitions
// Important - always use the full pin definition i.e. PXn where X is the port letter and n the pin number
const int ledPinBlue = PIN_PA3;// Pin 3 Port A
const int ledPinYellow = PIN_PB2;// Pin 2 Port B (TXD)
const int ledPinGreen = PIN_PB3;// Pin 3 Port B (RXD)
const int loopFrequency = PIN_PB1;// Pin 1 Port B, used to measure loop frequency using an oscilloscope

/* Define watchdog timer reset period
0x0 OFF    -
0x1 8CLK   0.008s
0x2 16CLK  0.016s
0x3 32CLK  0.031s
0x4 64CLK  0.063s
0x5 128CLK 0.125s
0x6 256CLK 0.25s
0x7 512CLK 0.5s
0x8 1KCLK  1s
0x9 2KCLK  2s
0xA 4KCLK  4s
0xB 8KCLK  8s
other - Reserved
*/
//const uint8_t reset_timeout = 0xB; //8-bit unsigned integer, maximum timeout is 8 seconds

// Variable definitions
unsigned int A = 888; //dividend
unsigned int B = 444; //divisor
unsigned int C = 666; //dividend
unsigned int D = 222; //divisor
unsigned int E = 1300; //dividend
unsigned int F = 555; //divisor

void setDefaultPinStates() {
  pinMode(ledPinBlue, OUTPUT); //set as output
  pinMode(ledPinYellow, OUTPUT); //set as output
  pinMode(ledPinGreen, OUTPUT); //set as output
  digitalWrite(ledPinBlue,HIGH); //start with LED off
  digitalWrite(ledPinYellow,HIGH); //start with LED off
  digitalWrite(ledPinGreen,HIGH); //start with LED off
  pinMode(loopFrequency, OUTPUT); //set as output
}

void setup() {
  setDefaultPinStates(); //run function
  // Configure watchdog timer, for more info see: "E:\Cloud Storage\Dropbox\Personal\Electronics\MCUs & Peripherals\Atmel - Microchip\CPUs\ATTiny - UPDI\ATTiny1614\Using the Watchdog timer.docx"
  //wdt_enable(reset_timeout); //Enable watchdog timer to reset MCU after reset_timer seconds, unless timer is reset before timeout period
  //wdt_enable(WDTO_2S); // Valid options are: WDTO_15MS, WDTO_30MS, WDTO_60MS, WDTO_120MS, WDTO_250MS, WDTO_500MS, WDTO_1S & WDTO_2S
  //wdt_enable(WDT_PERIOD_8KCLK_gc);
  _PROTECTED_WRITE(WDT.CTRLA,WDT_PERIOD_4KCLK_gc); // no window, 4 seconds
  
  delay (1000);
}

void loop() {
/*
  Flash the LEDs on PA2 & PB1 with a single line of code.
  The remainder of millis divided by A is compared to B
  and when greater the result is 1 and when less is 0, this binary result
  is written to the appropriate ledPin
*/
  digitalWrite(ledPinBlue, millis() % A > B);
  digitalWrite(ledPinYellow, millis() % C > D);
  digitalWrite(ledPinGreen, millis() % E > F);

  digitalWrite(loopFrequency, !digitalRead(loopFrequency)); //when enables the loop time can be viewed on pin PB1 using an oscilloscope

//Simulate a condition where the code gets stuck, which prevents the watchdog timer from being reset
  while (1) {
// This loop will execute indefinitely, causing the code to hang
  }

  // Reset the watchdog timer
  __asm__ __volatile__ ("wdr"::);
  //wdt_reset();  //Reset watchdog timer to avoid reseting the MCU
}

