int XRES = 4;
int YRES = 8;
int RECT_SIZE = 40;
int RECT_SIZE2 = RECT_SIZE + 10;
int OFS = 50;
int buffer[] = new int[XRES*YRES];
int[] discoOfs = new int[] {8,88,144,48,180,244,24,136,172,16,228,112,80,204,152,56,188,36,168,72,236,128,220,32,80,40,252104,64,196,212,120,160};

int mode = 0;
ColorSet cs, csColor, csWhite;
int frames=0;

void setup() {
  size(400, 500);
  background(0);
  frameRate(20);
  smooth();

  csColor = new ColorSet("AIDA", new int[] { 
    color(255, 0, 0), color(128, 0, 128), color(0, 0, 255), color(128, 0, 128) });
  csWhite = new ColorSet("BW", new int[] { 
    color(0, 0, 0), color(0, 0, 0), color(255, 255, 255), color(255, 255, 255) });
  cs = csWhite;
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
  
//  frames++;
  frames++;
}

  
void generateBuffer() {
  int ofs=0;

  switch (mode) {
  case 0:
    int col=0;    
    for (int y=0; y<YRES; y++) {      
      for (int x=0; x<XRES; x++) {
        buffer[ofs++] = (col+frames/2)%255;
      }
      col += 8;
    }
    cs = csColor;
    break;
    
  case 1:
    int n = (frames*16)%255;
    for (int i=0; i<32; i++) {
      buffer[i] = n+discoOfs[i];
    }
    cs = csColor;
    break;
  }
}



void keyPressed() {
  if (key != 'm') {
    return;
  }
  mode++;
  if (mode==0) {
    frames=0; 
  }
  if (mode>1) {
    mode=0;
  }
}


