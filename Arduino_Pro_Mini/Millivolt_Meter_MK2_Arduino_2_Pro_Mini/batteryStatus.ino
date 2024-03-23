//--- Battery Voltage Status Routine --------------------------------------------------------
void batteryStatus(void) {
  battery = analogRead(analogInput);  // read the value at analog input
  vout = (battery * 5.0) / 1024.0;    //
  batteryVolts = vout / (R2 / (R1 + R2));
  lcd.setCursor(1, 0);
  lcd.print("Battery Status");
  lcd.setCursor(3, 1);
  lcd.print(batteryVolts, 2);
  lcd.print(" Volts");
  delay(INTRO_DELAY_MS);
  lcd.clear();
}