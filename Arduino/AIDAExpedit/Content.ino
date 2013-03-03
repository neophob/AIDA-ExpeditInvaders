
unsigned long lastDrawTimestamp;

boolean timeOutNotReached(unsigned long lastTime, int timeout) {
  unsigned long time = millis()-lastTime;
  return time < timeout;
}


void generateContent() {

  if (timeOutNotReached(lastDrawTimestamp, ANIMATION_DELAY)) {
    return;
  }
  
  for (int i=0; i < NUM_LEDS; i++) {
    switch (contentMode) {
      case 0:
        //high heel mode
        break;
      case 1:
        //disco mode
        break;
    }  
  }
}
