/*   16 character 2 line I2C Display
 *   Uses Bill Perry's HD44780 Library, which can be installed from the Arduino Library Manager
 *   See Bills documentation: https://github.com/duinoWitchery/hd44780
 *   See the "API Summary" section.  Use "lcd" in front like lcdbegin(cols, rows)
 */

/*LiquidCrystal compatibility:
Since this hd44780 library is LiquidCrystal API compatible, most existing LiquidCrystal
sketches should work with hd44780 hd44780_I2Cexp i/o class once the
includes are changed to use hd44780 and the lcd object constructor is
changed to use the hd44780_I2Cexp i/o class.
*/

/*-----( Import needed libraries )-----*/
#include <Wire.h>
#include <hd44780.h>                        // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // i2c expander i/o class header

unsigned long time_for_action;

float freq = 15.0;  // default square wave frequency in kHz for testing the capacitors
int sqwPin = 2;     // defines D2 for square wave output signal

int voltageA0 = 0;
int voltageA1 = 0;
int resetADC;  // A2 grounded to reset ADC

int pinButton = 10;  // defines D10 for pullup button switch
int Relay = 13;      // defines D13 as the relay control pin
int stateRelay = LOW;
int stateButton;
int previous = LOW;
unsigned long time = 0; //unsigned long avoids compiler error
unsigned long debounce = 500;  // default of 500 ms works well  //unsigned long avoids compiler error
int stayON = 1000;    // keeps relay on (HIGH) for 1000 ms or 1 second

/*-----( Declare Constants )-----*/
// LCD geometry
const int LCD_COLS = 16;
const int LCD_ROWS = 2;

/*-----( Declare objects )-----*/
//hd44780_I2Cexp lcd;  // declare lcd object: auto locate & auto config expander chip
 hd44780_I2Cexp lcd(0x27);

/*-----( Declare Variables )-----*/
int status;

void setup() /*----( SETUP: RUNS ONCE )----*/
{
  pinMode(13, OUTPUT);   //set pin 13 to be an output
  
  status = lcd.begin(LCD_COLS, LCD_ROWS);
  if (status)  // non zero status means it was unsuccesful
  {
    // hd44780 has a fatalError() routine that blinks an led if possible
    // begin() failed so blink error code using the onboard LED if possible
    hd44780::fatalError(status);  // does not return
  }

    // Print a message to the LCD.
  lcd.clear();
  delay(1500);
  lcd.setCursor(0, 0);
  lcd.print("Capacitor foil");
  delay(1500);
  lcd.setCursor(0, 1);
  lcd.print("sniffer");

    // Define I/O pins
  pinMode(pinButton, INPUT);  // defines D10 as an INPUT to activate the button switch
  pinMode(Relay, OUTPUT);     // defines D13 as an OUTPUT to control the relay
  pinMode(5, OUTPUT);         // defines D5 as an OUTPUT to drive the right facing LED
  pinMode(6, OUTPUT);         // defines D6 as an OUTPUT to drive the left facing LED

  // Send message to Serial monitor
  Serial.begin(9600);
  Serial.print("Test Frequency: ");
  Serial.print(freq);
  Serial.println(" kHz");

} /*--(end setup )---*/


void loop() /*----( LOOP: RUNS CONSTANTLY )----*/
{

    // Sniffer code start

  stateButton = digitalRead(pinButton);
  //if (stateButton == HIGH && previous == LOW & millis() - time > debounce) { //needs parentheses, see next line
  if ((stateButton == HIGH && previous == LOW) & (millis() - time > debounce)) {
    if (stateRelay == HIGH) {
      digitalWrite(Relay, LOW);
    } else {
      digitalWrite(Relay, HIGH);
      delay(stayON);

      resetADC = analogRead(A2);  // A2 is grounded to clear the ADC before reading A0 & A1

      int sensorValue = analogRead(A0);
      float voltageA0 = sensorValue * (5.0 / 1023.0);
      Serial.print("A0: ");
      Serial.println(voltageA0, 4);

      if (voltageA0 > voltageA1) {
        digitalWrite(6, LOW);  // D6 off - (left facing LED)
      } else if (voltageA0 < voltageA1) {
        digitalWrite(6, HIGH);  // D6 on - (left facing LED) to indicate Foil Side of Capacitor
      }
      if (voltageA0 > voltageA1) {
        digitalWrite(5, HIGH);  // D5 on - (right facing LED) to indicate Foil Side of Capacitor
      } else if (voltageA0 < voltageA1) {
        digitalWrite(5, LOW);  // D5 off - (right facing LED)
      }
      digitalWrite(Relay, LOW);  // D13 off

      delay(100);  // small delay to reduce rollover samples

      int sensorValue1 = analogRead(A1);
      float voltageA1 = sensorValue1 * (5.0 / 1023.0);
      Serial.print("A1: ");
      Serial.println(voltageA1, 4);

      if (voltageA0 > voltageA1) {
        digitalWrite(6, LOW);  // D6 off - (left facing LED)
      } else if (voltageA0 < voltageA1) {
        digitalWrite(6, HIGH);  // D6 on - (left facing LED) to indicate Foil Side of Capacitor
      }
      if (voltageA0 > voltageA1) {
        digitalWrite(5, HIGH);  // D5 on - (right side facing LED) to indicate Foil Side of Capacitor
      } else if (voltageA0 < voltageA1) {
        digitalWrite(5, LOW);  // D5 off - (right facing LED)
      }
    }
    time = millis();

  }
  // previous == stateButton; //warning: statement has no effect [-Wunused-value]

  tone(sqwPin, freq * 1000); //sets test frequency
 
} /* --(end main loop )-- */

/*-----( Declare User-written Functions )-----*/


/* ( THE END ) */
