/*
  This blink sketch is for the LilyGo ESP32-S2 v1.1 dev board, ESP32-S2 is a highly integrated, low-power, single-core Wi-Fi Microcontroller SoC.
  Ensure that some version of the ESP32-S2 board (e.g. ESP32S2 Dev Module) is selected as the upload will fail if a standard ESP32 board is selected.
  If sketch does not run after uploading just press the reset button on the module.
  */
const byte led_gpio = 45;  //The const keyword stands for constant. It is a variable qualifier making a variable "read-only".

void setup() {

  pinMode(led_gpio, OUTPUT);  //Set led_gpio, or GPIO 45, as an output.
}

void loop() {
  digitalWrite(led_gpio, HIGH);  // turn the LED on (HIGH is the voltage level)
  delay(500);                    // wait for a second
  digitalWrite(led_gpio, LOW);   // turn the LED off by making the voltage LOW
  delay(1000);                   // wait for a second
}