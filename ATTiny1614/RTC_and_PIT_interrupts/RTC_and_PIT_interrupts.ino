/*
See: https://forum.arduino.cc/t/attiny1614-timer-b-issue/1216822
Sketch uses macros from the header file iotn1614.h, which can be viewed by right clicking
e.g. RTC_PER = 300; is a macro and is the same as directly writing to the register using RTC.PER = 300;
*/

const int ledPinBlue = PIN_PA3;    // Pin 3 Port A
const int ledPinYellow = PIN_PB2;  // Pin 2 Port B (TXD)

void setDefaultPinStates() {
  pinMode(ledPinBlue, OUTPUT);       //set as output
  pinMode(ledPinYellow, OUTPUT);     //set as output
  digitalWrite(ledPinBlue, HIGH);    //start with LED off
  digitalWrite(ledPinYellow, HIGH);  //start with LED off
}

void setup() {
  cli(); // Disables all interrupts by clearing the global interrupt mask, which is reset from reset anyway..
  setDefaultPinStates();  // run function
  F_CPU_init();           // reconfigure CPU clock prescaler

  //Configure clock source used by both PIT and RTC
  //RTC_CLKSEL = RTC_CLKSEL_INT32K_gc; /* Internal 32kHz OSC */
  RTC_CLKSEL = RTC_CLKSEL_INT1K_gc; /* Internal 1kHz OSC */

  //Configure RTC
  while (RTC_STATUS & RTC_PERBUSY_bm) {}  // wait until RTC_PER is available for writing
  RTC_PER = 300;                          // write period (16bit) to period register using built-in macro, or write directly to register using RTC.PER = 100;
                                          // i.e. using 1KHz (=1mS) internal oscillator an interrupt will be triggered every 100mS
  RTC_INTCTRL |= RTC_OVF_bm;  // activate overflow interrupts
  RTC_CTRLA |= RTC_RTCEN_bm;  // turn on RTC by setting bit 0 in Control Reg A (CTRLA) to a 1 by performing bitwise OR between CTRLA and RTC_RTCEN_bm defined as 0x01

  //Configure PIT
  RTC_PITINTCTRL |= RTC_PI_bm;                           // activate PIT Interrupt enable
  RTC_PITCTRLA |= RTC_PERIOD_CYC1024_gc | RTC_PITEN_bm;  // set PIT period and enable
  // The Realtimer-Counter (RTC) has its own 32,768KHz ultra Low-Power (ULP) internal RC oscillator, accuracy +/- 30% over full voltage/temp range.
  // 32.768KHz / CYCprescale = periodtime in mS
  //
  //  BIN         NAME        TIME
  // 0b00001001  CYC4        0,122 mS
  // 0b00011001  CYC8        0,244 mS
  // 0b00011001  CYC16       0,488 mS
  // 0b00100001  CYC32       0,977 mS
  // 0b00101001  CYC64       1,953 mS
  // 0b00110001  CYC128      3,906 mS
  // 0b00111001  CYC256      7,813 mS
  // 0b01000001  CYC512      15,63 mS
  // 0b01001001  CYC1024     31,25 mS
  // 0b01010001  CYC2048     62,50 mS
  // 0b01011001  CYC4096    125,00 mS
  // 0b01100001  CYC8192    250,00 mS
  // 0b01101001  CYC16384   500,00 mS
  // 0b01110001  CYC32768  1000,00 mS
  sei();  // turn on global interrupts
}

void loop() {
  // put your main code here, to run repeatedly:
}

ISR(RTC_PIT_vect) {                                        // RTC Interrupt
  RTC_PITINTFLAGS = RTC_PI_bm;                             // clear  Interrupt Flag
  digitalWrite(ledPinYellow, !digitalRead(ledPinYellow));  //toggle Yellow LED
}

ISR(RTC_CNT_vect) {                                    // RTC Interrupt
  RTC_INTFLAGS = RTC_OVF_bm;                           // clear  Interrupt Flag
  digitalWrite(ledPinBlue, !digitalRead(ledPinBlue));  //toggle Blue LED
}

void F_CPU_init() {
  // reconfigure CPU clock prescaler
#if (F_CPU == 20000000)
  /* No division on clock */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0x00);
#elif (F_CPU == 16000000)
  /* No division on clock */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, 0x00);
#elif (F_CPU == 10000000)  // 20MHz prescaled by 2
  /* Clock DIV2 */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_2X_gc));
#elif (F_CPU == 8000000)   // 16MHz prescaled by 2
  /* Clock DIV2 */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_2X_gc));
#elif (F_CPU == 5000000)   // 20MHz prescaled by 4
  /* Clock DIV4 */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_4X_gc));
#elif (F_CPU == 4000000)   // 16MHz prescaled by 4
  /* Clock DIV4 */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_4X_gc));
#elif (F_CPU == 1000000)   // 16MHz prescaled by 16
  /* Clock DIV16 */
  _PROTECTED_WRITE(CLKCTRL_MCLKCTRLB, (CLKCTRL_PEN_bm | CLKCTRL_PDIV_16X_gc));
#else
#ifndef F_CPU
#error "F_CPU not defined"
#else
#error "F_CPU defined as an unsupported value for untuned internal oscillator"
#endif
#endif
}