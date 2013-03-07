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
*/

static PROGMEM prog_uchar led_mapping[NUM_VISIBLE_LEDS] = {
//  104,105,106,107,108,109,110,111,112,113,114,115 FLIP
  115,114,113,112,111,110,109,108,107,106,105,104,

  89,90,91,92,93,94,95,96,97,98,99,100,

//  74,75,76,77,78,79,80,81,82,83,84,85, FLIP
  85,84,83,82,81,80,79,78,77,76,75,74,

  62,63,64,65,66,67,68,69,70,71,72,73,

//  44,45,46,47,48,49,50,51,52,53,54,55, FLIP
  55,54,53,52,51,50,49,48,47,46,45,44,

  29,30,31,32,33,34,35,36,37,38,39,40, 

//  14,15,16,17,18,19,20,21,22,23,24,25, FLIP
  25,24,23,22,21,20,19,18,17,16,15,14,

  //bottom line
   2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,
};


unsigned long lastDrawTimestamp = 0;

boolean timeOutNotReached(unsigned long lastTime, unsigned int timeout) {
  unsigned long time = millis()-lastTime;
  return time < timeout;
}


void generateContent() {

  if (timeOutNotReached(lastDrawTimestamp, ANIMATION_DELAY)) {
    return;
  }
  
  lastDrawTimestamp = millis();
  
  //generate buffer content
  for (int i=0; i < BUFFER_SIZE; i++) {
    switch (contentMode) {
      case 0:
        //fadein
        buffer[i] = 0+i;
        break;
      case 1:
        //high heel mode
        buffer[i] = 100+i;
        break;
      case 2:
        //disco mode
        buffer[i] = 200+i;
        break;
    }  
  }
  
  byte srcOfs=0;
  //map buffer to output
  for (unsigned int i=0; i<NUM_VISIBLE_LEDS; i++) {
    if (i%LED_GROUP==0) {
      srcOfs++;
    }
    
    unsigned long col = getSmoothColor(buffer[srcOfs]);    

    byte ofs = pgm_read_byte(&led_mapping[i]);
    //the first pixel is unused
    leds[ofs].r = (col>>16)&255;
    leds[ofs].g = (col>>8)&255; 
    leds[ofs].b = col&255;     

#ifdef USE_SERIAL_DEBUG
    //if (i==1) {
      Serial.print("Loop: ");
      Serial.print(i, DEC);
      Serial.print(", pyspos: ");
      Serial.print(ofs, DEC);
      Serial.print(", color: ");
      Serial.println(col, HEX);      
    //}
#endif
    
  }
  
  FastSPI_LED.show(); 
}


void initMode(byte mode) {
    unsigned long initialColor[3];
    switch (mode) {
      case 0:
        initialColor[0] = 0xffffff;  //white to black
        initialColor[1] = 0x7f7f7f;
        initialColor[2] = 0x000000;
        loadColorSet(initialColor);
        break; 
      case 1:
        initialColor[0] = 0xff0000;  //
        initialColor[1] = 0x00ff00;
        initialColor[2] = 0x0000ff;
        loadColorSet(initialColor);
        break; 
      case 2:
        initialColor[0] = 0xff0000;
        initialColor[1] = 0x00ff00;
        initialColor[2] = 0x0000ff;
        loadColorSet(initialColor);
        break; 
    }  
}
