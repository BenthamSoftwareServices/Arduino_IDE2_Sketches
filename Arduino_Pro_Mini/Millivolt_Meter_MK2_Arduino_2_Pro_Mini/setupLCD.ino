void setupLCD(void) {
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if (status)  // non zero status means it was unsuccesful
  {
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status);  // does not return
  }
}