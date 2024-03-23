// simple project using Arduino UNO and 16x2 character display to display "Hello World" 

// Links related to this project:
// Arduino UNO - https://s.click.aliexpress.com/e/_AXDw1h
// Arduino breadboard prototyping shield - https://s.click.aliexpress.com/e/_ApbCwx
// 16x2 displays with IIC - https://s.click.aliexpress.com/e/_9Hl3JV
// 16x2 display with RGB backlight - https://s.click.aliexpress.com/e/_9wgpeb
// Arduino I2C scanner - https://playground.arduino.cc/Main/I2cScanner/
// 16x2 available characters - https://docs.wokwi.com/parts/wokwi-lcd1602#font

// Organising your code with multiple tabs and h files = https://www.youtube.com/watch?v=PWMOb85OGY0

// How to use includes = https://www.arduino.cc/reference/en/language/structure/further-syntax/include/

#include "LCDhelper.h" //local file (not used at present)

#include <Wire.h> // Library file
// Bill Perry's extensible LCD library for HD44780 based LCD displays
#include <hd44780.h> // Library file, main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // Library file, i2c expander i/o class header

/*-----( Declare Constants )-----*/
// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

// Variables
int Count = 0; //A number to display and increment
int status; // holds returned status after lcd.begin if needed

/*-----( Declare objects )-----*/
//hd44780_I2Cexp lcd;  // declare lcd object: and specify lcd address as below:
 hd44780_I2Cexp lcd(0x27);
// if you don´t know the I2C address of the display, use I2C scanner first (https://playground.arduino.cc/Main/I2cScanner/)

void setup() {

  
  Serial.begin(9600);
  //PowerCycle(); // Power down. delay and power up LCD display to ensure full reset under software control to fix any garbage displayed characters
  lcd.setExecTimes(7000, 1600);
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  delay(10);
  Serial.print("status is ");
  Serial.println(status);
  lcd.backlight();   // Turn on backlight of LCD display screen.
//lcd.noBacklight(); // Turn off backlight of LCD display screen.
  lcd.clear(); // Clear display
  delay (10);
  // Commonly used instruction codes
  // lcd.clear();               // Clear LCD display screen
  // lcd.home();                // Go to home position (0,0) - column 1 & row 1
  // lcd.setCursor(col,row);    // Go to position (column, row)
  // lcd.print("text");         // Print text start from specified (column, row)
  lcd.setCursor(0, 0);          // Go to position column 0 & row 0
  lcd.print("No crash display");   // Print "Hello, World!"    
  lcd.setCursor(0, 1);          // Go to position column 0 & row 1
  lcd.print("Count ");          // Print "Count "
  lcd.print(Count);             // Print the value of the variable Count  
}
 
void loop() {

  //PowerCycle(); // Power down. delay, power up & initialise LCD display to ensure full reset under software control to fix any garbage displayed characters
  
  //lcd.setCursor(0, 0);          // Go to position column 0 & row 0
  //lcd.print("No crash display");   // Print "Hello, World!"    
  //lcd.setCursor(0, 1);          // Go to position column 0 & row 1
  //lcd.print("Count ");          // Print "Count "
  //lcd.print(Count);             // Print the value of the variable Count
  //Count ++;
  delay (1000);
}

void PowerCycle() {
  pinMode(2, OUTPUT); // +5v Vcc to power LCD module
  digitalWrite(2, LOW); // Power down LCD
  delay (100);
  digitalWrite(2, HIGH); // Power up LCD
  delay(10);
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  delay(10);
  lcd.backlight();   // Turn on backlight of LCD display screen.
//lcd.noBacklight(); // Turn off backlight of LCD display screen.
  lcd.clear(); // Clear display
  delay (10);  
}
