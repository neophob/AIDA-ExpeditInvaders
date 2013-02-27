void updateButtonState(){
  buttonState = digitalRead(INPUT_BUTTON_PIN); 
}

boolean isButtonPressed() {
  return buttonState==HIGH;
}
