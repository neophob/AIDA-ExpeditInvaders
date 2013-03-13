long buttonLastChecked = 0; // variable to limit the button getting checked every cycle
byte noBounce = 0;

void updateButtonState(){
  
  if( millis() - buttonLastChecked < BUTTONDELAY ) { // make sure a reasonable delay passed
    return;
  }
  buttonLastChecked = millis();
  
  //buttonState = digitalRead(INPUT_BUTTON_PIN);  
  buttonState = analogRead(INPUT_BUTTON_PIN);   // read the input pin

  if (buttonState > 512 && noBounce==0) {
    noBounce = 1;
    contentMode++;
    if (contentMode > MAX_MODE-1)  {
       contentMode=0; 
    }
#ifdef USE_SERIAL_DEBUG
  Serial.print("Button pressed, new mode: ");
  Serial.print(contentMode, DEC);
  Serial.println("");
#endif

    initMode(contentMode);
  } else if (buttonState < 513) {
     noBounce = 0; 
  }
  
/*
#ifdef USE_SERIAL_DEBUG
  Serial.print("Button state: ");
  Serial.print(buttonState, DEC);
  Serial.print(" current mode: ");
  Serial.print(contentMode, DEC);
  Serial.println("");
#endif
*/
  
}


