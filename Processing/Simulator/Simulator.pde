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
    color(0, 0, 0), color(255, 255, 255) });
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
  
  frames++;
  frames++;
}

  
void generateBuffer() {
  int ofs=0;

  switch (mode) {
  case 0:
    int ycnt = (frames/8)%5;
    int yofs = 7*XRES;
    int xofs = 0;
    
    //all white
    for (int y=0; y<YRES*XRES; y++) {
      buffer[y] = 128;
    }
        
    for (int y=0; y<YRES/2; y++) {
      ofs = y*XRES;
      for (int x=0; x<XRES/2; x++) {        
        if (ycnt>=y || (ycnt)>(x+1)) {
          buffer[ofs] = 0;
          buffer[ofs+yofs] = 0;
        }
        ofs++;
      }
      yofs -= 2*XRES;      
    }

    //mirror x
    for (int y=0; y<YRES; y++) {
      ofs = y*XRES;
      buffer[ofs+3] = buffer[ofs]; 
      buffer[ofs+2] = buffer[ofs+1]; 
    }    
    cs = csWhite;
    if (ycnt==4) {
      mode++;
    }
    break;
    
  case 1:
    int col=0;    
    for (int y=0; y<YRES; y++) {      
      for (int x=0; x<XRES; x++) {
        buffer[ofs++] = (col+frames)%255;
      }
      col += 8;
    }
    cs = csColor;
    break;
    
  case 2:
    int n = frames%255;
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
  if (mode>2) {
    mode=0;
  }
}


