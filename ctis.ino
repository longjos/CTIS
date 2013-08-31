#define cs 10
#define dc 8
#define rst -1
#define BUTTON_NONE 0
#define BUTTON_DOWN 1
#define BUTTON_RIGHT 2
#define BUTTON_SELECT 3
#define BUTTON_UP 4
#define BUTTON_LEFT 5


#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <CTIS.h>
#include <SPI.h>


uint8_t readButton(void) {
  float a = analogRead(3);
  
  a *= 5.0;
  a /= 1024.0;
  
  if (a < 0.2) return BUTTON_DOWN;
  if (a < 1.0) return BUTTON_RIGHT;
  if (a < 1.5) return BUTTON_SELECT;
  if (a < 2.0) return BUTTON_UP;
  if (a < 3.2) return BUTTON_LEFT;
  else return BUTTON_NONE;
}


Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);
GUICursor my_cursor(&tft, 159, 30, 30);
InflatorUI inflator_ui(&tft);


void draw_options(void){
  tft.setCursor(5, 1);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(4);
  tft.println("AIR");
  tft.setCursor(5, 31);
  tft.println("AIR");
  tft.setCursor(5, 61);
  tft.println("CRAWL");
  
  tft.fillTriangle(130, 1, 110, 27, 150, 27, ST7735_GREEN);
  tft.fillTriangle(110, 32, 150, 32, 130, 57, ST7735_RED);
  my_cursor.set_position_count(3);
}

bool in_progress = false;
uint8_t cursor_pos = 0;
uint8_t measured_pressure = 0;
uint8_t set_pressure = 32;
uint8_t current_status = STATUS_HOLD;

void setup(void) {
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setTextWrap(false);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(1);
  draw_options();
  my_cursor.draw_cursor();
}

void loop(void) {
  uint8_t b = readButton();

  if (b == BUTTON_LEFT) {
    cursor_pos = my_cursor.move_cursor(false);
  }
  if (b == BUTTON_RIGHT) {
    cursor_pos = my_cursor.move_cursor(true);
  }
  if (b == BUTTON_SELECT) {
    if(in_progress){
     tft.fillScreen(ST7735_BLACK);
     draw_options();
     my_cursor.draw_cursor();
     in_progress = false; 
    } else {
      tft.fillScreen(ST7735_BLACK);
      inflator_ui.draw(set_pressure, measured_pressure, current_status, true);
      in_progress = true;
    }
  }
  if(in_progress){
    inflator_ui.draw(set_pressure, measured_pressure, current_status);
    measured_pressure++;   
  }

  delay(200);
}





