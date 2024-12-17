#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

/* Uncomment the initialize the I2C address , uncomment only one, If you get a totally blank screen try the other*/
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
//#define i2c_Address 0x3d //initialize with the I2C addr 0x3D Typically Adafruit OLED's

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

int text_size = 1;

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define CHAR CHAR_WIDTH*text_size
#define LINE CHAR_HEIGHT*text_size
#define SEC 1000

#define btn 2

int screen_width;
int screen_height;
int value;
int sample_count = 0;
int delay_time = 500;

// dsm501a
#define pin2_1um 7
#define pin4_25um 8
#define sample_time 5000

float calc_low_ratio(float lowPulse) {
  return lowPulse / sample_time * 100.0;  // low ratio in %
}

float calc_c_mgm3(float lowPulse) {
  float r = calc_low_ratio(lowPulse);
  float c_mgm3 = 0.00258425 * pow(r, 2) + 0.0858521 * r - 0.01345549;
  return max(0, c_mgm3);
}

float calc_c_pcs283ml(float lowPulse) {
  float r = calc_low_ratio(lowPulse);
  float c_pcs283ml =  625 * r;
  return min(c_pcs283ml, 12500);
}

unsigned long init_time;

void setup() {
  Serial.begin(9600);

  delay(250); // wait for the OLED to power up
  display.begin(i2c_Address, true); // Address 0x3C default
  // display.setContrast (0); // dim display
  display.display();
  delay(2000);
  // Clear the buffer.
  display.clearDisplay();

  screen_width = display.width();
  screen_height = display.height();

  text_size = 2;
  display.setTextSize(text_size);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.println("HUMAM");
  display.display();

  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(btn, INPUT);
  pinMode(pin2_1um, INPUT);
  pinMode(pin4_25um, INPUT);

  init_time = millis();
  // delay(60*SEC);
}

void loop() {
  // digitalWrite(LED_BUILTIN, HIGH);
  // delay(100);
  // digitalWrite(LED_BUILTIN, LOW);
  // delay(100);

  static unsigned long t_start = millis();
  static float lowPM25, lowPM1 = 0;

  lowPM25 += pulseIn(pin4_25um, LOW) / 1000.0;
  lowPM1 += pulseIn(pin2_1um, LOW) / 1000.0;
  sample_count++;

  if ((millis() - t_start) >= sample_time) {
    Serial.print("t:");
    Serial.print((millis() - t_start)/1000.0);
    Serial.print(" ");

    Serial.print("samples:");
    Serial.print(sample_count);
    Serial.print(" ");

    Serial.print("r_2.5um:");
    Serial.print(calc_low_ratio(lowPM25));
    Serial.print(" ");
    Serial.print("mgm3_2.5um:");
    Serial.print(calc_c_mgm3(lowPM25));
    Serial.print(" ");
    Serial.print("pcs_2.5um:");
    Serial.print(calc_c_pcs283ml(lowPM25));
    Serial.print(" ");

    Serial.print("r_1um:");
    Serial.print(calc_low_ratio(lowPM1));
    Serial.print(" ");
    Serial.print("mgm3_1um:");
    Serial.print(calc_c_mgm3(lowPM1));
    Serial.print(" ");
    Serial.print("pcs_1um:");
    Serial.print(calc_c_pcs283ml(lowPM1));
    Serial.print(" ");

    Serial.println();

    // reset
    lowPM25 = 0;
    lowPM1 = 0;
    sample_count = 0;
    t_start = millis();
  }
}


