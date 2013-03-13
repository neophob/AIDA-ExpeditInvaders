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


static PROGMEM prog_uchar discoOfs[32] = 
  {8,88,144,48,180,244,24,136,
   172,16,228,112,80,204,152,56,
   188,36,168,72,236,128,220,32,
   80,40,252,104,64,196,212,120};



unsigned long lastDrawTimestamp = 0;
unsigned int frames = 0;

boolean timeOutNotReached(unsigned long lastTime, unsigned int timeout) {
  unsigned long time = millis()-lastTime;
  return time < timeout;
}


void generateContent() {
  int col, x, y, ofs;
  
  //return if delay not reached
  if (timeOutNotReached(lastDrawTimestamp, ANIMATION_DELAY)) {
    frames++;
    return;
  }
  
  lastDrawTimestamp = millis();
  
  //generate buffer content
    switch (contentMode) {
      case 0:
        //high heel mode
        col=0; 
        ofs=0;
        for (y=0; y<YRES; y++) {      
          for (x=0; x<XRES; x++) {
            buffer[ofs++] = (col+frames/2)%255;
          }
          col += 8;
        }
        break;
      case 1:
        //disco mode
        int n = (frames*4)%255;
        for (ofs=0; ofs<BUFFER_SIZE; ofs++) {          
          buffer[ofs] = n+pgm_read_byte(&discoOfs[ofs]);
        }
        
        break;
    }  

  
  byte srcOfs=0;
  //map buffer to output
  for (unsigned int i=0; i<NUM_VISIBLE_LEDS; i++) {
    
    //get current color
    unsigned long col = getSmoothColor(buffer[srcOfs]);    
    
    ///correct offset -1
    byte ofs = pgm_read_byte(&led_mapping[i])-1;

    leds[ofs].r = (col>>16)&255;
    leds[ofs].g = (col>>8)&255; 
    leds[ofs].b = col&255;     

/*#ifdef USE_SERIAL_DEBUG
    //if (i==1) {
      Serial.print("Loop: ");
      Serial.print(i, DEC);
      Serial.print(", pyspos: ");
      Serial.print(ofs, DEC);
      Serial.print(", srcOfs: ");
      Serial.print(srcOfs, DEC);
      Serial.print(", buffer: ");
      Serial.print(buffer[srcOfs], DEC);            
      Serial.print(", color: ");
      Serial.println(col, HEX);      
    //}
#endif*/

    if (i%LED_GROUP==(LED_GROUP-1)) {
      srcOfs++;
    }    
  }
  
  FastSPI_LED.show(); 
  frames++;
}


//load color set for each mode
void initMode(byte mode) {
    unsigned long initialColor[4];
    switch (mode) {
      case 0:
      case 1:
      //ff0000 -> blue
      //00ff00 -> green
/*        initialColor[0] = 0x00ff00;  //
        initialColor[1] = 0x00ff00;
        initialColor[2] = 0x000000;
        initialColor[3] = 0x0;*/
        initialColor[0] = 0xff0000;  //
        initialColor[1] = 0x7f007f;
        initialColor[2] = 0x0000ff;
        initialColor[3] = 0x7f007f;
        loadColorSet(initialColor);
        break; 
    }  
}

