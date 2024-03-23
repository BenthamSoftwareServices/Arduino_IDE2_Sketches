/*
An example of using the Hangul LCD library
*/

// Put and use copy of library in sketch folder, which does not auto-update, use double quotes not angle brackets
// The library must be removed from the installed folder at E:\Users\Steven\Documents\GitHub\Arduino_2\libraries or else the compiler may use either.

#include "LiquidCrystal_I2C_Hangul.h"
#include <Wire.h>

const uint8_t LCD_ADDRESS = 0x27;
const uint8_t LCD_COLUMNS = 16;
const uint8_t LCD_ROWS = 2;

unsigned long seconds;
unsigned long minutes;
unsigned long hours;
unsigned long currentMillis;

void calculate_elapsed_time();  //prototype for function to avoid rare compilation errors, see https://arduino.github.io/arduino-cli/0.20/sketch-build-process/

LiquidCrystal_I2C_Hangul lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void setup() {
  Serial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.clear();
  delay(1000);
  lcd.setCursor(0, 0);  //Column starting at 0, Row 0 for top and 1 for bottom
  lcd.print("Time since start");
  delay(1000);
  Serial.println("Setup complete");
}

void loop() {

  calculate_elapsed_time(); //function is located in file calculate_elapsed_time.ino split off from main ino file

  Serial.print("Time since start: ");  // print a message
  Serial.print(hours);                 // print hours
  Serial.print(":");
  Serial.print(minutes);  // print minutes
  Serial.print(":");
  Serial.println(seconds);  // print seconds

  delay(1000);
  display_time_elapsed_on_LCD();
}

void display_time_elapsed_on_LCD() {
  lcd.setCursor(0, 1);  //Column starting at 0, Row 0 for top and 1 for bottom

  if (hours < 10) {  // if hours is less than 10, print a leading zero
    lcd.print("0");
  }
  lcd.print(hours);
  lcd.print(":");

  if (minutes < 10) {  // if minutes is less than 10, print a leading zero
    lcd.print("0");
  }
  lcd.print(minutes);
  lcd.print(":");

  if (seconds < 10) {  // if seconds is less than 10, print a leading zero
    lcd.print("0");
  }
  lcd.print(seconds);
}
/*
void calculate_elapsed_time() {
  currentMillis = millis();        // get the current number of milliseconds
  seconds = currentMillis / 1000;  // convert milliseconds to seconds
  minutes = seconds / 60;          // convert seconds to minutes
  hours = minutes / 60;            // convert minutes to hours
  seconds %= 60;                   // calculate remaining seconds after minutes are removed
  minutes %= 60;                   // calculate remaining minutes after hours are removed
}
*/