#include <Arduino.h>  // Not necessary for the Arduino IDE but not a bad idea to always use for multi-compiler compatibility
#include <U8x8lib.h>
#include <Wire.h> // For I2C

/*
U8x8lib.h is included in U8g2 library, which is installed in Arduino IDE 2.xx, by Oliver Kraus olikraus@gmail.com
Dont email him use the forum at: https://github.com/olikraus/u8g2/issues

U8g2: Library for monochrome displays, version 2, latest version from: https://github.com/olikraus/u8g2

U8g2 is a monochrome graphics library for embedded devices. U8g2 supports monochrome OLEDs and LCDs, which include the following controllers:
SSD1305, SSD1306, SSD1309, SSD1312, SSD1316, SSD1320, SSD1322, SSD1325, SSD1327, SSD1329, SSD1606, SSD1607, SH1106, SH1107, SH1108, SH1122,
T6963, RA8835, LC7981, PCD8544, PCF8812, HX1230, UC1601, UC1604, UC1608, UC1610, UC1611, UC1617, UC1638, UC1701, ST7511, ST7528, ST7565,
ST7567, ST7571, ST7586, ST7588, ST75256, ST75320, NT7534, ST7920, IST3020, IST7920, LD7032, KS0108, KS0713, HD44102, T7932, SED1520,
SBN1661, IL3820, MAX7219 (see https://github.com/olikraus/u8g2/wiki/u8g2setupcpp for a full list).

The Arduino library U8g2 can be installed from the library manager of the Arduino IDE. U8g2 also includes U8x8 library:

U8g2
Includes all graphics procedures (line/box/circle draw).
Supports many fonts. (Almost) no restriction on the font height.
Requires some memory in the microcontroller to render the display.

U8x8
Text output only (character) device.
Only fonts allowed with fit into a 8x8 pixel grid.
Writes directly to the display. No buffer in the microcontroller required.
*/

U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* clock=*/PIN_WIRE_SCL, /* data=*/PIN_WIRE_SDA, /* reset=*/U8X8_PIN_NONE);  // OLEDs without Reset of the Display

void setup(void) {
  u8x8.setI2CAddress(0x78); // By default, U8x8 assumes the lowest possible I2C address of the display. This procedure will assign the I2C address to u8x8,
                            // if the display is configured to a different address. Call this procedure before begin().
                            // NOTE THAT ARM ADDRESSING SHOULD BE USED, NOT ARDUINO
                            // For more info see: https://github.com/olikraus/u8g2/wiki/u8x8reference#seti2caddress
  u8x8.begin();
  u8x8.clearDisplay();
  u8x8.setFlipMode(0);  // Set number to 0 for normal text, or 1 for upside-down text; note display ribbon cable is the bottom.
                        // It's essential to use ClearDisplay before FlipMode.
                        // See https://github.com/olikraus/u8g2/wiki/u8x8reference#setflipmode for more info.
}

void loop(void) {

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 0);
  u8x8.print("Hello Steven!");
  delay(1000);

  u8x8.inverse();

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 2);
  u8x8.print("Hello World!");
  delay(1000);

  u8x8.noInverse();

  u8x8.setFont(u8x8_font_chroma48medium8_r);
  u8x8.setCursor(0, 4);
  u8x8.print("System running");

  delay(1000);

  u8x8.clearDisplay();
  delay(1000);
}