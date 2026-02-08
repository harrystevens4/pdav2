// libraries
#include "ILI9341_LTSM.hpp"
// Fonts needed
#include <fonts_LTSM/FontOrla_LTSM.hpp>
#include <SPI.h>
#include "encoder.h"
#include "display16_graphics_LTSM.hpp"
#include <SD.h>

#define LCD_WIDTH 320   // Screen width in pixels
#define LCD_HEIGHT 240  // Screen height in pixels

int display_option_menu(const char **options, size_t option_count, const char *title = "");

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
	lcd_display.setFont(FontOrla);
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
}

// MAIN loop
void loop(void) {
	const char *main_menu[] = {"calendar","settings","ble","wifi"};
	int selection = display_option_menu(main_menu,sizeof(main_menu)/sizeof(char *),"Main menu");
	if (selection == 0){
		lcd_display.fillScreen(lcd_display.C_BLACK);
		lcd_display.setTextColor(0xffff, lcd_display.C_BLACK);
		lcd_display.setCursor(5,5);
		lcd_display.print("Calendar");
		delay(1000);
	}
}

int display_option_menu(const char **options, size_t option_count, const char *title){
	const int font_height = 24;
	const int font_width = 16;
	const int line_spacing = 5;
	//====== print title ======
	lcd_display.fillScreen(lcd_display.C_BLACK);
	lcd_display.setTextColor(0xffff, lcd_display.C_BLACK);
	lcd_display.setCursor(5,5);
	lcd_display.print(title);
	//====== render all the options ======
	lcd_display.setTextColor(0xbbbb, lcd_display.C_BLACK);
	for (size_t i = 0; i < option_count; i++){
		lcd_display.setCursor(15+(font_width*2), font_height+10 + (font_height+line_spacing)*i);
		lcd_display.print(options[i]);
	}
	int selection_index = 0;
	int previous_selection_index = 0;
	while (1){
		//====== render selection ======
		//unselect old selected index
		lcd_display.setTextColor(0xbbbb, lcd_display.C_BLACK);
		lcd_display.setCursor(15+(font_width*2), font_height+10 + (font_height+line_spacing)*previous_selection_index);
		lcd_display.print(options[previous_selection_index]);
		lcd_display.fillRect(0,font_height+10 + (font_height+line_spacing)*previous_selection_index,font_width+15,font_height,lcd_display.C_BLACK);
		previous_selection_index = selection_index;
		//select newly selected index
		lcd_display.setTextColor(0xffff, lcd_display.C_BLACK);
		lcd_display.setCursor(15+(font_width*2), font_height+10 + (font_height+line_spacing)*selection_index);
		lcd_display.print(options[selection_index]);
		lcd_display.setCursor(15, font_height+10 + (font_height+line_spacing)*selection_index);
		lcd_display.print(">");
		//====== await user input ======
	 	do {
			int button = dial1.get_button();
			if (button) return selection_index;
			int direction = dial1.get_direction();
			if (direction != 0){
				selection_index += direction;
				if (selection_index < 0) selection_index = option_count-1;
				selection_index %= option_count;
				break;
			}
		} while (1);
	}
}
