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

// SCULLCOM HOBBY ELECTRONICS
// Millivolt Meter MK2
// MILLIVOLT METER USING LTC2400 24bit ADC CHIP
// Software version 33.00 (uses I2C LCD 16x2 Display)
// Uses PCB designed by Greg Christenson [BARBOURI] which is available from OSH Park
// This PCB is available worldwide (free shipping) from OSH Park for just a few dollars. Check their link below for this project:
// https://oshpark.com/shared_projects/qgv0fpKN
// The PCB design file is also available on this webpage to download.
// https://644db4de3505c40a0444-327723bce298e3ff5813fb42baeefbaa.ssl.cf1.rackcdn.com/0fa8729a07e906a84e6cf5762d45a390.brd

// 4.096 volt precision reference (ADR4540)
//LTC2400 SCK to digital pin 13
//LTC2400 SDO to digital pin 12
//LTC2400 CS  to digital pin 10

/*-----( Import needed libraries )-----*/
#include <Wire.h>                           // Library allows you to communicate with I2C devices [A4 (SDA), A5 (SCL)]
#include <hd44780.h>                        // main hd44780 header
#include <hd44780ioClass/hd44780_I2Cexp.h>  // i2c expander i/o class header
#include <EEPROM.h>                         // include EEPROM library
#include <SPI.h>                            // Serial Peripheral Interface Library used to communicate with LTC2400 (MISO, MOSI and SCK)

// --- PIN assignments ------------------------------------------------------------------------

#define LTC2400_CS 10   // chip select pin for ADC
#define BUTTON_CLEAR 2  // D2 used for Clear Calibration Memory button
#define BUTTON_CAL 3    // D3 used for Calibration button
#define BUTTON_DEC 4    // D4 used for Number of Decimal Places button
#define BUTTON_HOLD 5   // D5 used for Hold Current Reading on display
#define BUTTON_BAR 6    // D6 used to toggle Bar Graph Voltage Display (max 16 volt)

/*-----( Declare Constants )-----*/
// LCD geometry and address
const int LCD_COLS = 20; //16 for 16 x 2 LCD
const int LCD_ROWS = 4; //2 for 16 x 2 LCD
const int LCD_ADDRESS = 0x3F; //0x27 for 16 x 2 LCD

// --- Constants ------------------------------------------------------------------------------

#define NUMBER_OF_SAMPLES 5               // Number of samples to average (was 5)
#define VOLTAGE_REFERENCE 4.096           // voltage reference used for LTC2400
#define MAIN_LOOP_DELAY_MS 300             // value determines voltage reading update rate (1000 will give a slow update option, original value was 30)
#define DEBOUNCE_DELAY_MS 250             // value determines button debounce delay was 150
#define INTRO_DELAY_MS 5000               // intro screen delays (originally 2000)
#define CALIBRATION_PROMPT_DELAY_MS 5000  // calibration prompt delay time (originally 2000)

// --- Global variables -----------------------------------------------------------------------

uint32_t g_samples[NUMBER_OF_SAMPLES] = { 0 };
uint32_t g_current_sample = 0;
uint8_t g_number_of_decimals = 6;

int holdButtonState = 0;
int eeprom_Address = 0;
uint32_t calibration_offset = 0;

float barVoltage = 0;
int barGraphState = 0;
float barVolt2 = 0;
int barVolt1 = 0;
float barDiffVolts = 0;

int status;

//--- Battery Status variables -----------------------------------------------------------------------

int analogInput = 0;  // sets analog input to A0 for battery check
int battery = 0;
float vout = 0.0;
float batteryVolts = 0.0;
float R1 = 1000000.0;  // resistance of R1 (1M)    // part of resistor divider for battery status check
float R2 = 100000.0;   // resistance of R2 (100K)   // part of resistor divider for battery status check

//--- Bar Graph Special Characters Bit Map------------------------------------------------------------

byte p20[8] = {
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
  B10000,
};
byte p40[8] = {
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
  B11000,
};
byte p60[8] = {
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
  B11100,
};
byte p80[8] = {
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
  B11110,
};
byte p100[8] = {
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
  B11111,
};

/*-----( Declare objects )-----*/
//hd44780_I2Cexp lcd;  // declare lcd object: auto locate & auto config expander chip
hd44780_I2Cexp lcd(LCD_ADDRESS);

// --- Setup function (only runs once) --------------------------------------------------------
void setup(void) {
  setupIOPins();
  setupSPIBus();
  setupLCD();

  barGraphCharacter();  //bar graph character bit map setup

  showIntro();
  batteryStatus();

  calibration_offset = (EEPROMreadlong(eeprom_Address));
  showCalibrationData();
}

// --- Main Programme loop ------------------------------------------------------------------------------
void loop(void) {
  if (digitalRead(BUTTON_CLEAR) == LOW) {  // Pull-down on board; active HIGH
    calibrationClear();

  } else if (digitalRead(BUTTON_DEC) == LOW) {  // Internal pull-up used; active LOW
    adjustDecimalPlaces();

  } else if (digitalRead(BUTTON_HOLD) == LOW) {  // Internal pull-up used; active LOW
    holdReading();

  } else if (digitalRead(BUTTON_BAR) == LOW) {  // Internal pull-up used; active LOW
    barGraphSetup();

  } else if (digitalRead(BUTTON_CAL) == LOW) {  // Internal pull-up used; active LOW
    calibrationTrim();

  } else {

    readADC();      // read voltage from ADC
    showReading();  // Display voltage on LCD
  }
  delay(MAIN_LOOP_DELAY_MS);
}
// End of main program loop

// --- Convert ADC reading to Decimal Voltage reading -------------------------------------------------------
float convertToVoltage(uint32_t reading) {
  return reading * 10 * VOLTAGE_REFERENCE / 16777216;
}

//--- Read voltage from LTC2400 ADC --------------------------------------------------------------------------
unsigned long readADC(void) {
  digitalWrite(LTC2400_CS, LOW);  //LTC2400 chip select pin taken low to allow data transfer from ADC
  delayMicroseconds(10);          //timing delay but may not be required

  while ((PINB & (1 << 4))) {}  //check to see if ADC is ready by testing EOC - wait while conversion completed

  uint32_t reading = 0;
  for (int i = 0; i < 4; ++i) {     // Ready 4 bytes (32 bits) from the ADC
    reading <<= 8;                  // Before each readin shift the existing content over to make room
    reading |= SPI.transfer(0xFF);  // Read one byte
    if (i == 0)
      reading &= 0x0F;  // Discard 4 status bits of the first byte
  }

  reading >>= 4;  // Discard 4 left most sub LSB bits

  digitalWrite(LTC2400_CS, HIGH);  //LTC2400 chip select pin taken high disables ADC output.

  g_samples[g_current_sample++] = reading;    // Store value in the bucket used to calculate averages
  if (g_current_sample == NUMBER_OF_SAMPLES)  // Jump back to the first slot once we go past the last slot
    g_current_sample = 0;
}

//--- Calculate Voltage reading running average and add zero Calibration and trim Calibration factors ----
uint32_t getADCAverage(void) {
  uint32_t sum = 0;
  for (int i = 0; i < NUMBER_OF_SAMPLES; i++)
    sum += g_samples[i];  // Sum of all stored up samples

  sum = sum / NUMBER_OF_SAMPLES;   // Calculate average by dividing total readings value by number of samples taken
  sum = sum + calibration_offset;  // Add Calibration Correction Value
  return sum;
}

//--- Print Voltage Reading to Display -----------------------------------------------------------------
void showReading(void) {
  uint32_t reading = getADCAverage();
  float volt = convertToVoltage(reading);

  barVoltage = volt;  // added for bar graph option

  char prefix = 0;
  if (volt < 0.001) {
    volt = volt * 1000000;
    prefix = 'u';

  } else if (volt < 1) {
    volt = volt * 1000;
    prefix = 'm';
  }

  //lcd.setCursor(0, 1);                      // Jump to second line in the display (bottom)
  lcd.setCursor(0, 0);                    //
  lcd.print(volt, g_number_of_decimals);  // Print voltage as floating number with the right number of decimal places
  lcd.print(" ");                         // Add one blank space after voltage reading

  if (prefix)
    lcd.print(prefix);
  lcd.print("V        ");  // Extra spaces to clean up when voltages go from large to small (8 spaces)

  if (barGraphState == 1) {  //Display voltage bar graph if option selected
    barGraphDisplay();
  } else {
    delay(1);
  }
}

//--- Number of Decimal Places routine --------------------------------------------------------------------
void adjustDecimalPlaces(void) {
  ++g_number_of_decimals;
  if (g_number_of_decimals > 6)
    g_number_of_decimals = 0;

  delay(DEBOUNCE_DELAY_MS);  // Very simple de-bounce delay
}

//--- Voltage Bar Graph Routine ---------------------------------------------------------------
void barGraphDisplay(void) {
  barDiffVolts = (barVolt1 + barVolt2) - barVoltage;
  if (barDiffVolts > 0.1 || barDiffVolts < -0.1) {  // only refresh bar graph if voltage changed (stops ficker)

    barVolt1 = (int)barVoltage;        // voltage reading before the decimal point
    barVolt2 = barVoltage - barVolt1;  // voltage reading after the decimal point

    lcd.setCursor(0, 1);
    lcd.print("                ");          // clear second row with 16 spaces
    for (int i = 0; i < (barVolt1); ++i) {  // display full bars showing voltage reading before the decimal point
      lcd.setCursor(i, 1);                  // increment display cursor position
      lcd.write(5);                         // full bar segment character
    }
    if (barVolt2 > 0.9) {          // if decimal value is greater than 0.9v
      lcd.setCursor(barVolt1, 1);  // increment display cursor position by one
      lcd.write(5);                // display full 5 line segments of character block

    } else if (barVolt2 > 0.7) {   // if decimal value is greater than 0.7v
      lcd.setCursor(barVolt1, 1);  // increment display cursor position by one
      lcd.write(4);                // display 4 line segments of character block

    } else if (barVolt2 > 0.5) {   // if decimal value is greater than 0.5v
      lcd.setCursor(barVolt1, 1);  // increment display cursor position by one
      lcd.write(3);                // display 3 line segments of character block

    } else if (barVolt2 > 0.3) {   // if decimal value is greater than 0.3v
      lcd.setCursor(barVolt1, 1);  // increment display cursor position by one
      lcd.write(2);                // display 2 line segments of character block

    } else if (barVolt2 > 0.1) {   // if decimal value is greater than 0.1v
      lcd.setCursor(barVolt1, 1);  // increment display cursor position by one
      lcd.write(1);                // display 1 line segments of character block
    }
  }
}

//--- Hold Current Voltage Reading and Display ---------------------------------------------------------------
void holdReading(void) {

  if (holdButtonState == 0) {

    barGraphState = 0;  //Clear bar graph if active

    lcd.setCursor(0, 1);
    lcd.print("                ");  //clear second row with 16 spaces
    lcd.setCursor(0, 1);

    uint32_t reading = getADCAverage();
    float volt = convertToVoltage(reading);

    char prefix = 0;
    if (volt < 0.001) {
      volt = volt * 1000000;
      prefix = 'u';

    } else if (volt < 1) {
      volt = volt * 1000;
      prefix = 'm';
    }

    lcd.setCursor(0, 1);                    // Jump to second line in the display (bottom)
    lcd.print(volt, g_number_of_decimals);  // Print voltage as floating number w/ the right number of decimal places
    lcd.print(" ");                         // Add one blank space after voltage reading

    if (prefix)
      lcd.print(prefix);
    lcd.print("V    ");  // Extra spaces to clean up when voltages go from large to small

    lcd.setCursor(15, 1);
    lcd.print("H ");

    holdButtonState = 1;  // set hold button state for next press

    delay(DEBOUNCE_DELAY_MS);  // Very simple de-bounce delay

  } else if (holdButtonState == 1) {  // Toggle hold button state
    lcd.setCursor(0, 1);
    lcd.print("                ");  //clear second row with 16 spaces
    holdButtonState = 0;
    delay(DEBOUNCE_DELAY_MS);  // Very simple de-bounce delay
  }
}

//----Calculate Calibration Value and save in eeprom -------------------------------------------------------------------------
void calibrationTrim(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Apply 5v Ref");
  delay(1000);

  calibration_offset = 0;                  // Reset Trim calibration value
  for (int i = 0; i < NUMBER_OF_SAMPLES;)  // Get multiple measurements
  {
    if (readADC()) {  // Wait for the ADC measurements to be ready
      ++i;
      lcd.print('.');
    }
  }
  calibration_offset = getADCAverage();               // Read new average value
  calibration_offset = 2048000 - calibration_offset;  // 5 volt reference is equal to and ADC value of 2048000
  EEPROMwritelong(eeprom_Address, calibration_offset);
  calibration_offset = (EEPROMreadlong(eeprom_Address));

  lcd.setCursor(0, 1);
  lcd.print("Cal. Data = ");
  lcd.print(calibration_offset);
  delay(3000);
  lcd.clear();
}

//--- Clear Calibration Data -------------------------------------------------------
void calibrationClear(void) {
  uint32_t calibration_offset = 0;
  EEPROMwritelong(eeprom_Address, calibration_offset);
  lcd.setCursor(0, 1);
  lcd.print("Cal Data Cleared = ");
  calibration_offset = (EEPROMreadlong(eeprom_Address));
  delay(CALIBRATION_PROMPT_DELAY_MS);
  lcd.setCursor(0, 1);
  lcd.print("                ");
}

//--- Bar Graph on/off function ----------------------------------------------------------------
void barGraphSetup(void) {
  if (barGraphState == 0) {
    barGraphState = 1;
    barVolt1 = 0;
    barVolt2 = 0;
  } else {
    barGraphState = 0;
    lcd.setCursor(0, 1);
    lcd.print("                ");
  }
  delay(DEBOUNCE_DELAY_MS);  // Very simple de-bounce delay
}

//--- EEPROM routine used for saving Calibration Data -------------------------------------

// routine to write a 4 byte (32 bit) long to EEPROM at specified addresses
void EEPROMwritelong(int eeprom_Address, long value) {
  byte four = (value & 0xFF);  //four = least significant byte
  byte three = ((value >> 8) & 0xFF);
  byte two = ((value >> 16) & 0xFF);
  byte one = ((value >> 24) & 0xFF);  //one = most significant byte

  EEPROM.write(eeprom_Address, four);  //write the four bytes into EEPROM
  EEPROM.write(eeprom_Address + 1, three);
  EEPROM.write(eeprom_Address + 2, two);
  EEPROM.write(eeprom_Address + 3, one);
}

// routine to read back 4 bytes and return with (32 bit) long as value
long EEPROMreadlong(long eeprom_Address) {
  long four = EEPROM.read(eeprom_Address);  //read the 4 bytes from EEPROM
  long three = EEPROM.read(eeprom_Address + 1);
  long two = EEPROM.read(eeprom_Address + 2);
  long one = EEPROM.read(eeprom_Address + 3);
  return (four) + (three << 8) + (two << 16) + (one << 24);
}
//**************************************************************************************************
