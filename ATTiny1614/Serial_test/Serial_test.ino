
// Variable definitions


void setDefaultPinStates() {
delay(1);
}

void setup() {
  setDefaultPinStates(); //run function
  Serial.begin(9600);
  
  delay (1000);
}

void loop() {
Serial.print("xyz");
Serial.print(" ");
delay (1000);
}