/*================================= EG LABS =======================================
Not working, but see sketch SPI_Output_demo

Loop Back test for the SPI port of the Arduino

 The circuit:

 * LED attached from pin 6 to ground through a 1K resistor
================================== EG LABS =======================================*/

#include <SPI.h>                                                  // including the SPI library

char outByte [20] = "ENGINEERS GARAGE";                           // string to be send and received via SPI port
char inByte;                                                      // variable which stores the value of the byte received from SPI bus
int i = 0;                                                         

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);                                       // setting the LED pin as output
  Serial.begin(9600);                                             // initializing the serial port at 9600 baud rate
  SPI.begin();                                                    // initialize the SPI port as master
  delay(100);
}

void loop() 
{    

    
  digitalWrite(LED_BUILTIN, HIGH);          
  //for(i = 0; outByte [i] != ''; i ++)                            // send and receive the bytes of the string
  for(i = 0; outByte [i] < 10 ; i ++) 
  {
    inByte = SPI.transfer(outByte [i]);
    Serial.write(inByte);   
  }
 
  delay(1000);

  digitalWrite(LED_BUILTIN, LOW); 

  delay(1000);  
}
