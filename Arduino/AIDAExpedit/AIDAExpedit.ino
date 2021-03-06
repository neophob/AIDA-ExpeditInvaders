
/*

	   +----------+----------+----------+----------+  EXPEDIT REGAL#2
	   |          |          |          |          |
	   |          |          |          |          |
	   |          |          |          |          |       
	   |115 14 13 |112 11 10 |109 08 07 |106 05 04 |
	   +----------+----------+----------+----------+
	   |          |          |          |          | 103
	   |          |          |          |          | 102 
	   |          |          |          |          | 101
	   | 89 90 91 | 92 93 94 | 95 96 97 | 98 99 100|
	   +----------+----------+----------+----------+
	88 |          |          |          |          |
	87 |          |          |          |          |
	86 |          |          |          |          | 
	   | 85 84 83 | 82 81 80 | 79 78 77 | 76 75 74 | 
	   +----------+----------+----------+----------+ 
	   | 62 63 64 | 65 66 67 | 68 69 70 | 71 72 73 | 
	   |          |          |          |          | 
	   |          |          |          |          | 
	61 |          |          |          |          | 
	   +----------+----------+----------+----------+   
	60
	59 +----------+----------+----------+----------+  EXPEDIT REGAL#1
	58 |          |          |          |          |
	57 |          |          |          |          |
	56 |          |          |          |          |       
	   | 55 54 53 | 52 51 50 | 49 48 47 | 46 45 44 |
	   +----------+----------+----------+----------+
	   |          |          |          |          | 43
	   |          |          |          |          | 42
	   |          |          |          |          | 41      
	   | 29 30 31 | 32 33 34 | 35 36 37 | 38 39 40 |
	   +----------+----------+----------+----------+
	28 |          |          |          |          |
	27 |          |          |          |          |
	26 |          |          |          |          |      
	   | 25 24 23 | 22 21 20 | 19 18 17 | 16 15 14 | 
	   +----------+----------+----------+----------+ 
	   | 02 03 04 | 05 06 07 | 08 09 10 | 11 12 13 | 
	   |          |          |          |          | 
	01 |          |          |          |          | 
       PSU | ARDUNIO  |          |          |          |
	   +----------+----------+----------+----------+
 * AIDA Expedit Invaders Copyright (C) 2013 michael vogt <michu@neophob.com>
 */
#include <FastSPI_LED.h>
#include <avr/pgmspace.h>


// --------------------------------------------

//one pixel uses 3 leds
#define LED_GROUP 3

//total nr of led modules
#define NUM_LEDS 115

//xresolution 
#define XRES 4

//xresolution 
#define YRES 8

//effective buffer size
#define BUFFER_SIZE (XRES*YRES)

//not all modules are visible!  32*3=96
#define NUM_VISIBLE_LEDS (BUFFER_SIZE*LED_GROUP)

//which input pin is used to read the button
#define INPUT_BUTTON_PIN 5

//speed for the animation
#define ANIMATION_DELAY 5

//read the button state each X ms
#define BUTTONDELAY 150

//use serial debug or not
#define USE_SERIAL_DEBUG 1

//how many animation modi exist
#define MAX_MODE 2

// --------------------------------------------
//State of the button
int buttonState;

//display mode
byte contentMode = 0;

//internal frame buffer
byte buffer[BUFFER_SIZE];

// Sometimes chipsets wire in a backwards sort of way
struct CRGB { 
  unsigned char b; 
  unsigned char r; 
  unsigned char g; 
};
// struct CRGB { unsigned char r; unsigned char g; unsigned char b; };
struct CRGB *leds;


// --------------------------------------------
//      setup
// --------------------------------------------
void setup() {
#ifdef USE_SERIAL_DEBUG
  Serial.begin(115200);
  Serial.println("HI AIDA!");
#endif

  //init input
  pinMode(INPUT_BUTTON_PIN, INPUT);    // declare pushbutton as input
  buttonState = 0;
  
  //init leds
  FastSPI_LED.setLeds(NUM_LEDS);
  FastSPI_LED.setChipset(CFastSPI_LED::SPI_WS2801);

  //select spi speed, 7 is very slow, 0 is blazing fast
  FastSPI_LED.setDataRate(3); //try 3
  FastSPI_LED.init();
  FastSPI_LED.start();
  leds = (struct CRGB*)FastSPI_LED.getRGBData(); 
  
#ifdef USE_SERIAL_DEBUG
  Serial.println("Setup done");
#endif
  
  initMode(0);
}


void loop() {
  updateButtonState();
  
  generateContent();
  
  //if we use strands with differnt color order, fix this here
  swapGBColorOrderPerString(20);
  swapGBColorOrderPerString(100);
    
  FastSPI_LED.show(); 
}

//swap color order (Green and Blue) for specific stands
void swapGBColorOrderPerString(int ofs) {
  //one strand has 20 led modules
  uint8_t tmp;
  for (int i=0; i<20; i++) {
    tmp = leds[ofs].b;
    leds[ofs].b = leds[ofs].g; 
    leds[ofs].g = tmp;
    ofs++;
  }
}




