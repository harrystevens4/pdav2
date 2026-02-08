// libraries
#include "ILI9341_LTSM.hpp"
// Fonts needed
#include <fonts_LTSM/FontMega_LTSM.hpp>
#include "fonts_LTSM/FontGroTesk_LTSM.hpp"
#include <SPI.h>
#include "encoder.h"
#include "display16_graphics_LTSM.hpp"
#include <SD.h>

#define LCD_WIDTH 320   // Screen width in pixels
#define LCD_HEIGHT 240  // Screen height in pixels

ILI9341_LTSM lcd_display;
bool bhardwareSPI = true;  // true for hardware spi, false for software

SPIClass *vspi = NULL;
RotaryEncoder dial1(5, 22, 2);

void setup(void) {
  Serial.begin(9600);
  delay(1000);
  Serial.println("ready");
  vspi = new SPIClass(FSPI);
  Serial.print("cs: ");
  Serial.println(SS);
  vspi->begin(18, 19, 23, 10);
  //====== sd card initialisation ======
  uint8_t sd_cs = 10;
  int sd_ready = !SD.begin(sd_cs, *vspi);
  File settings_file = SD.open("/config", FILE_WRITE);
  if (settings_file) {
    size_t result = settings_file.println("pluh");
    Serial.printf("%lu bytes written\n",result);
    settings_file.close();
  }
  //====== lcd initialisation ======
  // === USER OPTION 1 SPI_SPEED + TYPE ===
  int8_t DC_TFT = 3;
  int8_t RST_TFT = 0;
  int8_t CS_TFT = 15;
  uint32_t TFT_SCLK_FREQ = 8000000;  // Spi freq in Hertz
  lcd_display.SetupGPIO_SPI(TFT_SCLK_FREQ, RST_TFT, DC_TFT, CS_TFT);
  lcd_display.ILI9341Initialize();
  lcd_display.SetupScreenSize(LCD_HEIGHT, LCD_WIDTH);
  lcd_display.setFont(FontGroTesk);
  lcd_display.setRotation(display16_graphics_LTSM::Degrees_90);
  lcd_display.fillScreen(lcd_display.C_BLACK);
  //====== display sd card errors ======
  pinMode(sd_cs, OUTPUT);
  if (sd_ready) {
    Serial.println("sd card failure");
    lcd_display.setCursor(40, 100);
    lcd_display.print("SD card error");
    while (1){
      ;
    }
  } else {
    Serial.println("sd card ready");
    Serial.printf("card size: %fGB\n",(float)SD.cardSize() / (1024*1024*1024));
  };
  lcd_display.setCursor(0, 15);
  lcd_display.print("pluh");
  delay(1000);
  lcd_display.fillScreen(lcd_display.C_BLACK);

}

// MAIN loop
void loop(void) {
  int selection_index = 0;
  while (1) {
    int direction = dial1.get_direction();
    if (direction != 0) {
      selection_index += direction;
      Serial.println(direction);
      lcd_display.fillRect(0, 0, LCD_WIDTH, 60, lcd_display.C_BLUE);
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