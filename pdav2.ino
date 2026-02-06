// libraries
#include "ILI9341_LTSM.hpp"
// Fonts needed
#include <fonts_LTSM/FontMega_LTSM.hpp>
#include "fonts_LTSM/FontGroTesk_LTSM.hpp"
#include <SPI.h>
#include "encoder.h"


ILI9341_LTSM lcd_display;
bool bhardwareSPI = true;  // true for hardware spi, false for software

SPIClass *vspi = NULL;
RotaryEncoder dial1(5, 22, 2);

void setup(void) {
  Serial.begin(9600);
  delay(1000);
  vspi = new SPIClass(FSPI);
  vspi->begin(18, 19, 23, 15);
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 3;
  int8_t RST_TFT = 0;
  int8_t CS_TFT = 15;
  uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
  lcd_display.SetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  // ===
  // === USER OPTION 2 Screen Setup ===
  uint16_t TFT_WIDTH = 240;   // Screen width in pixels
  uint16_t TFT_HEIGHT = 320;  // Screen height in pixels
  lcd_display.SetupScreenSize(TFT_WIDTH, TFT_HEIGHT);
  lcd_display.setFont(FontGroTesk);
  // ===
  lcd_display.ILI9341Initialize();
  Serial.println("Start");
  delay(1000);
}

// MAIN loop
void loop(void) {
  lcd_display.fillScreen(lcd_display.C_BLACK);
  int selection_index = 0;
  while (1) {
    int direction = dial1.get_direction();
    if (direction != 0) {
      selection_index += direction;
      Serial.println(direction);
      lcd_display.fillRect(0, 0, 240, 50, lcd_display.C_BLUE);
      lcd_display.setTextColor(lcd_display.C_WHITE, lcd_display.C_BLACK);
      lcd_display.setCursor(0, 15);
      lcd_display.print(String(selection_index));
    }
  }
}

void HelloWorld(void) {
  delay(7000);
}

void EndTests(void) {
  lcd_display.fillScreen(lcd_display.C_BLACK);
  lcd_display.PowerDown();  // Power down device
  Serial.println("End");
}