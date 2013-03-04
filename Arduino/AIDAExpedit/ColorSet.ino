//Generated with https://github.com/adafruit/RGB-matrix-Panel/blob/master/extras/gamma.c

static PROGMEM prog_uchar gamma[] = {
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
  0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,
  0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
  0x01,0x02,0x02,0x02,0x02,0x02,0x02,0x02,
  0x02,0x03,0x03,0x03,0x03,0x03,0x04,0x04,
  0x04,0x04,0x04,0x05,0x05,0x05,0x05,0x06,
  0x06,0x06,0x06,0x07,0x07,0x07,0x07,0x08,
  0x08,0x08,0x09,0x09,0x09,0x0a,0x0a,0x0a,
  0x0b,0x0b,0x0c,0x0c,0x0c,0x0d,0x0d,0x0e,
  0x0e,0x0f,0x0f,0x0f,0x10,0x10,0x11,0x11,
  0x12,0x12,0x13,0x13,0x14,0x14,0x15,0x16,
  0x16,0x17,0x17,0x18,0x19,0x19,0x1a,0x1a,
  0x1b,0x1c,0x1c,0x1d,0x1e,0x1e,0x1f,0x20,
  0x21,0x21,0x22,0x23,0x24,0x24,0x25,0x26,
  0x27,0x28,0x28,0x29,0x2a,0x2b,0x2c,0x2d,
  0x2e,0x2e,0x2f,0x30,0x31,0x32,0x33,0x34,
  0x35,0x36,0x37,0x38,0x39,0x3a,0x3b,0x3c,
  0x3d,0x3e,0x3f,0x40,0x41,0x43,0x44,0x45,
  0x46,0x47,0x48,0x49,0x4b,0x4c,0x4d,0x4e,
  0x50,0x51,0x52,0x53,0x55,0x56,0x57,0x59,
  0x5a,0x5b,0x5d,0x5e,0x5f,0x61,0x62,0x63,
  0x65,0x66,0x68,0x69,0x6b,0x6c,0x6e,0x6f,
  0x71,0x72,0x74,0x75,0x77,0x79,0x7a,0x7c,
  0x7d,0x7f,0x81,0x82,0x84,0x86,0x87,0x89,
  0x8b,0x8d,0x8e,0x90,0x92,0x94,0x96,0x97,
  0x99,0x9b,0x9d,0x9f,0xa1,0xa3,0xa5,0xa6,
  0xa8,0xaa,0xac,0xae,0xb0,0xb2,0xb4,0xb6,
  0xb8,0xba,0xbd,0xbf,0xc1,0xc3,0xc5,0xc7,
  0xc9,0xcc,0xce,0xd0,0xd2,0xd4,0xd7,0xd9,
  0xdb,0xdd,0xe0,0xe2,0xe4,0xe7,0xe9,0xeb,
  0xee,0xf0,0xf3,0xf5,0xf8,0xfa,0xfd,0xff
};


unsigned long colors[3];

//get array size
#define nrOfColorsInArray (sizeof(colors)/sizeof(unsigned long)) 

#define MAX_COLOR_MODE 10
byte colorMode = 0;

byte boarderCount;
byte arrayCount;

//----------------------------
// Color, convert 8bit to 5bit
//----------------------------
unsigned int Color(uint8_t r, uint8_t g, uint8_t b) {
  r = pgm_read_byte(&gamma[r]); // Gamma correction table maps
  g = pgm_read_byte(&gamma[g]); // 8-bit input to 4-bit output
  b = pgm_read_byte(&gamma[b]);
  uint32_t ret = (r << 16) | (g << 8) | b;
  return ret;   
}

//----------------------------
//init color set
//----------------------------
void initColorSet(unsigned long col[3]) {
  //copy array.. memcpy? never heard of it...
  for (byte b=0; b<3; b++) {
    colors[b] = col[b];
  }

  arrayCount = nrOfColorsInArray;
  boarderCount = 255 / arrayCount;
#ifdef USE_SERIAL_DEBUG
  Serial.print("boarderCount: ");
  Serial.println(boarderCount);
#endif 
}

//----------------------------
//calc smooth color
//----------------------------
unsigned int getSmoothColor(byte pos) {
  byte ofs=0;
  while (pos>boarderCount) {
    pos -= boarderCount;
    ofs++;
  }

  byte targetOfs = ofs+1;
  return calcSmoothColor(colors[targetOfs%arrayCount], colors[ofs%arrayCount], pos);
}

//----------------------------
//calc smooth color
//----------------------------
unsigned int calcSmoothColor(unsigned long col1, unsigned long col2, byte pos) {
  unsigned int b= col1&255;
  unsigned int g=(col1>>8)&255;
  unsigned int r=(col1>>16)&255;
  unsigned int b2= col2&255;
  unsigned int g2=(col2>>8)&255;
  unsigned int r2=(col2>>16)&255;

  unsigned int mul=pos*arrayCount;        //extend pos to 255, so pos is between 0..255
  unsigned int oppositeColor = 255-mul;

  r=(r*mul + r2*oppositeColor) >> 8;
  g=(g*mul + g2*oppositeColor) >> 8;
  b=(b*mul + b2*oppositeColor) >> 8;

  //apply gamma tab
  r=pgm_read_byte_near(gamma+r);
  g=pgm_read_byte_near(gamma+g);
  b=pgm_read_byte_near(gamma+b);

  //change next line
  return Color(b,r,g);
}

//----------------------------
//apply color
//----------------------------
void applyColorSet() {
  byte srcOfs = -1;

  for (int i=0; i < NUM_LEDS; i++) {
    if (i%LED_GROUP==0) {
      srcOfs++;
    }

    //make sure the blind pixels are ignored
    if (srcOfs==5) {
      srcOfs++;
    }
    unsigned int col = getSmoothColor(buffer[srcOfs]);    

    //the first pixel is unused
    leds[i].r = (col>>16)&255;
    leds[i].g = (col>>8)&255; 
    leds[i].b = col&255;     
  }

  //blank unused modules
/*  strip.setPixelColor(0, 0);
  strip.setPixelColor(13, 0);
  strip.setPixelColor(14, 0);
  strip.setPixelColor(15, 0);
*/
  FastSPI_LED.show();

}

//----------------------------
//load colorset
//----------------------------
void loadColorSet(byte colorMode) {
  unsigned long initialColor[3] = { 
    0xff0000, 0x00ff00, 0x0000ff   
  }; //RGB
 
  switch (colorMode) {
  case 1:
    initialColor[0] = 0xdc323c;  //Rasta
    initialColor[1] = 0xf0cb58;
    initialColor[2] = 0x3c825e;
    break; 
  case 2:
    initialColor[0] = 0xd3517d;  //CGA
    initialColor[1] = 0x15a0bf;
    initialColor[2] = 0xffc062;
    break; 
  case 3:
    initialColor[0] = 0x008c53;  //Brazil
    initialColor[1] = 0x2e00e4;
    initialColor[2] = 0xdfea00;  
    break; 
  case 4:
    initialColor[0] = 0x588F27;  //Fizz
    initialColor[1] = 0x04BFBF;
    initialColor[2] = 0xF7E967;  
    break; 
  case 5:
    initialColor[0] = 0x9f456b;  //Kitty
    initialColor[1] = 0x4f7a9a;
    initialColor[2] = 0xe6c84c;  
    break; 
  case 6:
    initialColor[0] = 0x323228;  //Neon
    initialColor[1] = 0x717155;
    initialColor[2] = 0xb4dc00;  
    break; 
  case 7:
    initialColor[0] = 0x000000;  //Lantern
    initialColor[1] = 0x0d9a0d;
    initialColor[2] = 0xffffff;  
    break; 
  case 8:
    initialColor[0] = 0x0000ff;  //Lemming
    initialColor[1] = 0x00ff00;
    initialColor[2] = 0xffffff;  
    break; 
  case 9:
    initialColor[0] = 0x3e3e3e;  //LeBron
    initialColor[1] = 0xd4b600;
    initialColor[2] = 0xffffff;  
    break; 
  }
#ifdef USE_SERIAL_DEBUG      
  Serial.print("[Right] load new color: ");
  Serial.println(colorMode);      
#endif
  initColorSet(initialColor);
}

