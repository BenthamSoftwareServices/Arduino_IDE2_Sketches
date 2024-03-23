/********* x
  Rui Santos
  Complete project details at https://randomnerdtutorials.com  
*********/

// Loops from address 1 to 127 and if no I2C transmission error the address is sent to serial

#include <Wire.h>

void setup() {
  Wire.begin();
  // deactivate internal pullups for twi.
  //digitalWrite(SDA, 0);
  //digitalWrite(SCL, 0);

  Serial.begin(9600);
  Serial.println("\nI2C Scanner");
}

void loop() {

  Serial.print("\x1B[1;1H\x1B[2J");  // ANSI VT100 Escape sequences to move terminal cursor to position 1,1 then clear the terminal window
  
  delay(1000);

  byte error, address;
  int nDevices;
  Serial.println("Scanning...");
  nDevices = 0;
  for (address = 1; address < 127; address++) {
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0) {
      Serial.print("I2C device found at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
      nDevices++;
    } else if (error == 4) {
      Serial.print("Unknow error at address 0x");
      if (address < 16) {
        Serial.print("0");
      }
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0) {
    Serial.println("No I2C devices found\n");
  } else {
    Serial.println("done\n");
  }
  delay(1000);
}