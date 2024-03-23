//---Intro Screen at Switch On -----------------------------------------------------------------
void showIntro(void) {
  lcd.clear();
  lcd.setCursor(4, 0);
  lcd.print("SCULLCOM");
  lcd.setCursor(0, 1);
  lcd.print("Hobby Electronic");
  delay(INTRO_DELAY_MS);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Millivolt Meter");
  lcd.setCursor(0, 1);
  lcd.print("Software Ver. 33");
  delay(INTRO_DELAY_MS);
  lcd.clear();
}