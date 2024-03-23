/*
This sketch transmits and receives data via my SPI test sheild board

The schematic design is in EasyEDA at: https://oshwlab.com/steveben/arduino-uno-spi-test-demo-board

Note that the SPI library defines certain pin names that can be used without further definition e.g.
MISO Pin 12  The Slave line for receiving data to the master
MOSI Pin 11  The Master line for sending data to the peripherals
SCK  Pin 13  The clock pulses which synchronize data transmission
SS   Pin 10  Slave Select - the pin on each device that the master can use to enable and disable specific devices.
             When a device's Slave Select pin is low, it communicates with the master.
             When it's high, it ignores the master. This allows you to have multiple SPI devices sharing the same MISO, MOSI, and CLK lines.
The four SPI pins for the Arduino UNO are defined in SPI.h and do not need further definition             
*/

#include <SPI.h>  // including the SPI library

int tx_data = 0x84;  // SPI data to transmit, from 0 to 255 decimal, or 0x00 to 0xFF
int rx_data = 52;     // Variable to receive incoming SPI data, unnecessarily set to zero

int yellow_led = 6;  // Pin number D6 at which the Yellow LED is attached, do not use 13/LED_BUILTIN as this is used for SPI clock.

void setup() {

  pinMode(yellow_led, OUTPUT);  // initialize digital pin led (6) as an output.
  Serial.begin(9600);           // Start Serial ready to send data to TTY monitor
  SPI.begin();                  // Start SPI function
  delay(100);
}

void loop() {

  SPI.beginTransaction(SPISettings(200000, MSBFIRST, SPI_MODE0));  // Prepare for an SPI transaction.
                                                                   //speedMaximum: The maximum speed of communication. For an SPI chip rated up to 20 MHz, use 20000000. Not all speed settings work e.g. 200,000 or less gives SPI clock of 125KHz, 2,000,000 gives 2MHz.
                                                                   //dataOrder: MSBFIRST or LSBFIRST
                                                                   //dataMode: SPI_MODE0, SPI_MODE1, SPI_MODE2, or SPI_MODE3

  digitalWrite(SS, LOW);  // Set Slave Select LOW to enable the peripheral to recieve data e.g. connected via 74HC00 to S0 on the FCT299 to enable shift right function and OE to disable outputs to LEDs, during shifting

  //delay (5);
  delayMicroseconds(5);

  // transmit the value of tx_data on MOSI and receive DIP switch data into rx_data, all at the same time
  rx_data = SPI.transfer(tx_data);

  //delay(5);
  delayMicroseconds(5);

  digitalWrite(SS, HIGH);  // Set Slave Select HIGH to disable the peripheral to recieve data e.g. connected via 74HC00 to S0 on the FCT299 to enable hold function and OE to enable outputs to LEDs

  SPI.endTransaction();  // End the SPI transaction

  delay(100);

  Serial.print("The 8-bit DIP switch is set to: ");
  Serial.println(rx_data, HEX);

  digitalWrite(yellow_led, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(100);                      // wait for a second
  digitalWrite(yellow_led, LOW);   // turn the LED off by making the voltage LOW
  delay(100);                      // wait for a second


  tx_data = rx_data; //If enabled this causes the DIP switch setings to be displayed on the eight white LEDs
}