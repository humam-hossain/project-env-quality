#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

// happy birthday frequency
#define NOTE_G4  392
#define NOTE_A4  440
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_D5  587
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_G5  784

// Melody and note durations
int melody[] = {
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_C5, NOTE_B4,  // Line 1
  NOTE_G4, NOTE_G4, NOTE_A4, NOTE_G4, NOTE_D5, NOTE_C5,  // Line 2
  NOTE_G4, NOTE_G4, NOTE_G5, NOTE_E5, NOTE_C5, NOTE_B4, NOTE_A4, // Line 3
  NOTE_F5, NOTE_F5, NOTE_E5, NOTE_C5, NOTE_D5, NOTE_C5   // Line 4
};

int durations[] = {
  500, 500, 500, 500, 500, 1000,  // Line 1
  500, 500, 500, 500, 500, 1000,  // Line 2
  500, 500, 500, 500, 500, 500, 1000,  // Line 3
  500, 500, 500, 500, 500, 1000   // Line 4
};

// Lyrics for each line
String words[] = {
  "HAPPY", "BIRTHDAY", "TO", "YOU", 
  "HAPPY", "BIRTHDAY", "TO", "YOU", 
  "HAPPY", "BIRTHDAY", "DEAR", "MARPHY", 
  "HAPPY", "BIRTHDAY", "TO", "YOU"
};

// Number of notes each word spans
int notesPerWord[] = {
  2, 2, 1, 1, // Line 1
  2, 2, 1, 1, // Line 2
  2, 2, 1, 2, // Line 3
  2, 2, 1, 1  // Line 4
};

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
}

void loop() {
  // Play the melody
  text_size = 2;
  int wordIndex = 0;
  int noteCount = 0;

  // Play the melody and display synchronized words
  for (int i = 0; i < sizeof(melody) / sizeof(melody[0]); i++) {
    // Display the current word if it's the first note for that word
    if (noteCount == 0) {
      display.clearDisplay();
      display.setTextSize(text_size);
      display.setTextColor(SH110X_WHITE);
      display.setCursor(screen_width/2 - (words[wordIndex].length() * CHAR)/2, 1*LINE);
      display.print(words[wordIndex]); // Display the current word
      display.display();
    }

    // Play the note
    int noteDuration = durations[i];
    tone(buzzer, melody[i], noteDuration);
    delay(noteDuration * 1.3);
    noTone(buzzer);

    // Update note count and check if we need to move to the next word
    noteCount++;
    if (noteCount >= notesPerWord[wordIndex]) {
      noteCount = 0;
      wordIndex++;
    }
  }

  delay(5000); // Wait 5 seconds before replaying the song
}