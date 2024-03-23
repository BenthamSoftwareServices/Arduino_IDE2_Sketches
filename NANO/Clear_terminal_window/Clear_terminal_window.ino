// This example code shows how to clear a terminal windows (not Arduino IDE serial monitor!) and reset the cursor to a specific position
// Also hoe to change the format of the displayed text

void setup() {
  // open the serial port at 9600 bps:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.print("\x1B[1;1H\x1B[2J");  // ANSI VT100 Escape sequences to move terminal cursor to position 1,1 then clear the terminal window

  Serial.print("\x1b[1;31m");  // Set terminal style to bold, red foreground.  See https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797

  delay(1000); //Show terminal window is blank for 1 second
  
  Serial.println("Hello");

  delay(1000); // Leave text in terminal window for 1 second
}
