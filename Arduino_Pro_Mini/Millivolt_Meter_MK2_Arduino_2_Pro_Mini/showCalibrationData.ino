//--- Show Calibration Data -----------------------------------------------------------------
void showCalibrationData(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Cal. Data = ");
  lcd.setCursor(0, 1);  
  lcd.print(calibration_offset);
  delay(CALIBRATION_PROMPT_DELAY_MS);
  lcd.clear();
}
