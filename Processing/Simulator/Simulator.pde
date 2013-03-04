int XRES = 4;
int YRES = 8;
int RECT_SIZE = 40;
int RECT_SIZE2 = RECT_SIZE + 10;
int OFS = 50;
int buffer[] = new int[XRES*YRES];

int mode = 0;
ColorSet cs;
int frames=0;

void setup() {
  size(400, 500);
  background(0);
  frameRate(20);
  smooth();

  cs = new ColorSet("AIDA", new int[] 
  { 
    color(255, 0, 0), color(128, 0, 128), color(0, 0, 255), color(128, 0, 128)
  } 
  );
}




void draw() {
  background(0);

  generateBuffer();

  int ofs=0;
  for (int y=0; y<YRES; y++) {
    for (int x=0; x<XRES; x++) {
      fill(cs.getSmoothColor(buffer[ofs++]));
      rect(OFS+x*RECT_SIZE2, OFS+y*RECT_SIZE2, RECT_SIZE, RECT_SIZE);
    }
  }
  
  frames++;
  frames++;
  frames++;
  frames++;
}


void generateBuffer() {

  switch (mode) {
  case 0:
    int ofs=0;
    int col=0;
    for (int y=0; y<YRES; y++) {      
      for (int x=0; x<XRES; x++) {
        buffer[ofs++] = (col+frames)%255;
      }
      col += 8;
    }
    break;
  case 1:
    int n = frames%255;
    buffer[0] = n+8;
    buffer[1] = n+88;    
    buffer[2] = n+144;
    buffer[3] = n+48;    
    buffer[4] = n+180;
    buffer[5] = n+244;    
    buffer[6] = n+24;
    buffer[7] = n+136;    
    buffer[8] = n+172;
    buffer[9] = n+16;    
    buffer[10] = n+228;
    buffer[11] = n+112;    
    buffer[12] = n+80;
    buffer[13] = n+204;    
    buffer[14] = n+152;
    buffer[15] = n+56;    
    buffer[16] = n+188;
    buffer[17] = n+168;    
    buffer[18] = n+72;
    buffer[19] = n+236;    
    buffer[20] = n+128;
    buffer[21] = n+220;    
    buffer[22] = n+32;
    buffer[23] = n+80;    
    buffer[24] = n+40;
    buffer[25] = n+252;    
    buffer[26] = n+104;
    buffer[27] = n+64;    
    buffer[28] = n+196;
    buffer[29] = n+212;    
    buffer[30] = n+120;    
    buffer[31] = n+160;
    break;
  }
}



void keyPressed() {
  if (key != 'm') {
    return;
  }
  mode++;
  if (mode>1) {
    mode=0;
  }
}


