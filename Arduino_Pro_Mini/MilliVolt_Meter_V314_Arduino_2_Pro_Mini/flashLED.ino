void flashLED(void) {
  //debug test flash built in LED to stop code execution
int myLED = 9;
pinMode(myLED, OUTPUT);  // Initialize the LED pin as an output
   for (int i = 0; i <= 1000000; i++) {  // Iterate from 0 to 1000000 = 55.55 hours
    digitalWrite(myLED, HIGH);    // Turn on the LED
    delay(100);                         // Wait for 100 milliseconds
    digitalWrite(myLED, LOW);     // Turn off the LED
    delay(100);                         // Wait for another 100 milliseconds
  }
}


//.....................................................................................................................................debug start....//
// volt = (average - zero_cal) * v_ref /16777216 * 10;  

//  lcd.setCursor(0, 1);         // set LCD cursor to Column 0 and Row 1 (second row of LCD, first column)
//  lcd.print (volt, 6);
//  flashLED();  //debug test flash built in LED to stop code execution...................................................................flashLED....//  