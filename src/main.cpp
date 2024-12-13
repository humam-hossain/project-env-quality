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

int textSize = 1;

#define CHAR_WIDTH 6
#define CHAR_HEIGHT 8
#define CHAR CHAR_WIDTH*textSize
#define LINE CHAR_HEIGHT*textSize
#define SEC 1000

#define btn 2
#define buzzer 4

int screen_width;
int screen_height;
int value;
int count = 0;

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

  pinMode(btn, INPUT);
  pinMode(buzzer, OUTPUT);

  tone(buzzer, 300);
  delay(150);
  tone(buzzer, 400);
  delay(200);
  tone(buzzer, 500);
  delay(250);
  tone(buzzer, 600);
  
  noTone(buzzer);
}

void loop() {
  value = digitalRead(btn);
  if(value == HIGH) {
    tone(buzzer, 1000);
    delay(150);
    noTone(buzzer);

    count++;
  }

  textSize = 2;
  display.setTextSize(textSize);
  display.setTextColor(SH110X_WHITE);

  display.setCursor(0, 1*LINE);
  display.print("Marphy Bot");
  
  display.setTextColor(SH110X_BLACK, SH110X_WHITE);
  display.setCursor(0, 2*LINE);
  display.print("COUNT");

  display.setTextColor(SH110X_WHITE);
  display.setCursor(screen_width - 5/2*CHAR, 2*LINE);
  display.print(count);
  display.display();
  delay(100);
  display.clearDisplay();

  if(count >= 10){
    display.setCursor(screen_width/2 - CHAR*3/2, screen_height/2);
    display.println("K/O");
    display.display();
    tone(buzzer, 1000);
    delay(300);
    tone(buzzer, 700);
    delay(400);    
    
    display.clearDisplay();
    display.setCursor(screen_width/2 - CHAR*5/2, 1*LINE);
    display.print("MARHY");
    display.setCursor(screen_width/2 - CHAR*5/2, 2*LINE);
    display.print("LOSER");
    display.display();
    tone(buzzer, 300);
    delay(800);
    noTone(buzzer);
    display.clearDisplay();
    delay(2000);

    display.setCursor(screen_width/2 - CHAR*5/2, 1*LINE);
    display.print("HUMAM");
    display.setCursor(screen_width/2 - CHAR*5/2, 2*LINE);
    display.print("WINS!");
    display.display();
    tone(buzzer, 500);
    delay(300);
    tone(buzzer, 600);
    delay(300);
    tone(buzzer, 700);
    delay(300);
    tone(buzzer, 800);
    delay(300);
    tone(buzzer, 1000);
    delay(300);
    noTone(buzzer);
    display.clearDisplay();
    delay(2000);

    // restarting the game
    count = 0;
    display.setCursor(0, 0);
    display.print("Restarting...");
    display.display();
    delay(100);
    for(int i=3; i>0; i--){
      display.print(i);
      display.print(" ");
      display.display();
      tone(buzzer, 500);
      delay(150);
      noTone(buzzer);
      delay(850);
    }
    delay(1000);
    display.clearDisplay();
  }
}