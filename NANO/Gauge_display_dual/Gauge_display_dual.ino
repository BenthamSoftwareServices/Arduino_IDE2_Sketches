// simple project using Arduino UNO and 16x2 character display to display gauge, 
// created by upir, 2022
// youtube channel: https://www.youtube.com/upir_upir

// FULL TUTORIAL: https://youtu.be/upE17NHrdPc

// Links related to this project:
// Arduino UNO - https://s.click.aliexpress.com/e/_AXDw1h
// Arduino breadboard prototyping shield - https://s.click.aliexpress.com/e/_ApbCwx
// 16x2 displays with IIC - https://s.click.aliexpress.com/e/_9Hl3JV
// 16x2 display with RGB backlight - https://s.click.aliexpress.com/e/_9wgpeb
// original sketch from YWROBOT - https://wokwi.com/arduino/libraries/LiquidCrystal_I2C/HelloWorld
// character creator - https://tusindfryd.github.io/screenduino/
// another character creator - https://maxpromer.github.io/LCD-Character-Creator/
// sprintf explanation - https://www.programmingelectronics.com/sprintf-arduino/
// custom characters simplest project - https://wokwi.com/projects/294395602645549578
// Arduino I2C scanner - https://playground.arduino.cc/Main/I2cScanner/

/*-----( Import needed libraries )-----*/
#include <Wire.h>
#include <hd44780.h>                        // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // i2c expander i/o class header

/*-----( Declare Constants )-----*/
// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

/*-----( Declare objects )-----*/
//hd44780_I2Cexp lcd;  // declare lcd object: auto locate & auto config expander chip, otherwise specify lcd address as below:
 hd44780_I2Cexp lcd(0x27);
// define custom characters - every character is 5x8 "pixels"
byte gauge_left[8] =   {  B11111,  B10000,  B10000,  B10000,  B10000,  B10000,  B10000,  B11111};  // left part of empty gauge    [ 
byte gauge_center[8] = {  B11111,  B00000,  B00000,  B00000,  B00000,  B00000,  B00000,  B11111};  // center part of empty gauge  _
byte gauge_right[8] =  {  B11111,  B00001,  B00001,  B00001,  B00001,  B00001,  B00001,  B11111};  // right part of empty gauge   ]
byte gauge_fill[8] =   {  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111,  B11111};  // filled gauge                █

int cpu_gauge;    // value for the CPU gauge
int gpu_gauge;    // value for the GPU gauge
char buffer[10];  // helper buffer to store C-style strings (generated with sprintf function)

void setup()
{
  cpu_gauge = 45;  // set "random" value for CPU gauge
  gpu_gauge = 70;  // set "random" value for GPU gauge

  lcd.init();                       // initialize the 16x2 lcd module
  lcd.createChar(0, gauge_left);    // create special character on position 0, gauge left    [ 
  lcd.createChar(1, gauge_center);  // create special character on position 1, gauge center  _
  lcd.createChar(2, gauge_right);   // create special character on position 2, gauge right   ]
  lcd.createChar(3, gauge_fill);    // create special character on position 3, gauge fill    █
  lcd.backlight();                  // enable backlight for the LCD module
}

void loop()
{
  // CPU gauge  
  lcd.setCursor(0,0);                         // move cursor to top left
  sprintf(buffer, "CPU:%3d%%", cpu_gauge);    // set a string as CPU: XX%, with the number always taking at least 3 character
                                              // %3d means % is a character specifier, 3 specifies that 3 positions will be used for the data d i.e. the variable cpu_gauge
                                              // the %% means display one %, the first % is the format specifier                                               
  lcd.print(buffer);                          // print the string on the display, then continue to draw the gauge
 
  // 8 characters to draw the gauge --- ████___]
  float cpu_gauge_step = 100.0/8.0;         // 100% is the maximum number, gauge is 8 characters wide, calculate one step  
  for (int i=0; i<8; i++) {
    if (cpu_gauge <= cpu_gauge_step*i) {    // value is smaller than step*i, draw "empty" character
         if (i==0)      {lcd.write(0);}     // [     first cell, opening bracket
         else if (i==7) {lcd.write(2);}     // ]     last cell, closing bracket
         else           {lcd.write(1);}     // _     any other cell, lines top and bottom
       }
    else {                                  // value is bigger than step*i, draw filled character - █
         lcd.write(3);
       }
  }

  // GPU gauge
  lcd.setCursor(0,1);                         // move cursor to second line
  sprintf(buffer, "GPU:%3d%%", gpu_gauge);    // set a string as GPU: XX%, with the number always taking at least 3 character
                                              // %3d means % is a character specifier, 3 specifies that 3 positions will be used for the data d i.e. the variable gpu_gauge
                                              // the %% means display one %, the first % is the format specifier           
  lcd.print(buffer);                          // print the string on the display, then continue to draw the gauge
 
  // 8 characters to draw the gauge --- [______]
  float gpu_gauge_step = 100.0/8.0;         // 100% is the maximum number, gauge is 8 characters wide, calculate one step 
  for (int i=0; i<8; i++) {
    if (gpu_gauge <= gpu_gauge_step*i) {    // value is smaller than step*i, draw "empty" character
         if (i==0) {lcd.write(0);}          // [     first cell, opening bracket
         else if (i==7) {lcd.write(2);}     // ]     last cell, closing bracket
         else {lcd.write(1);}               // _     any other cell, lines top and bottom
       }
    else {                                  // value is bigger than step*i, draw filled character - █
         lcd.write(3);
       }
  }  

  // increase the CPU value, set between 0-100
  cpu_gauge = cpu_gauge +1;
  if (cpu_gauge > 100) {cpu_gauge = 0;}

  // incrase the GPU value, set between 0-100
  gpu_gauge = gpu_gauge +1;
  if (gpu_gauge > 100) {gpu_gauge = 0;}  

  delay(1000);  // wait for a while - 100ms = update the screen 10x in a second

}