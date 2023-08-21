// 6 consecutive digital pins for the LEDs
int first = 2;
int second = 3;
int third = 4;
int fourth = 5;

// pin for the button switch
int button = 12;
// value to check state of button switch
int pressed = 0;

void setup() {
  // set all LED pins to OUTPUT
  for (int i=first; i<=fourth; i++) {
    pinMode(i, OUTPUT);
  }
  // set buttin pin to INPUT
  pinMode(button, INPUT_PULLUP);
  
  // initialize random seed by noise from analog pin 0 (should be unconnected)
  randomSeed(analogRead(0));

}

void buildUpTension() {
  // light LEDs from left to right and back to build up tension
  // while waiting for the dice to be thrown
  // left to right
  for (int i=first; i<=fourth; i++) {
    if (i!=first) {
      digitalWrite(i-1, LOW);
    }
    digitalWrite(i, HIGH);
    delay(100);
  }
  // right to left
  for (int i=fourth; i>=first; i--) {
    if (i!=fourth) {
      digitalWrite(i+1, LOW);
    }
    digitalWrite(i, HIGH);
    delay(100);
  }
}

void showNumber(int number) {
  digitalWrite(first, HIGH);
  if (number >= 2) {
    digitalWrite(second, HIGH);
  }
  if (number >= 3) {
    digitalWrite(third, HIGH);    
  }
  if (number >= 4) {
    digitalWrite(fourth, HIGH);    
  }
}

int throwDice() {
  // get a random number in the range [1,6]
  int randNumber = random(1,5);
  
  return randNumber;
}

void setAllLEDs(int value) {
  for (int i=first; i<=fourth; i++) {
    digitalWrite(i, value);
  }
}

void loop() {
  if(analogRead(A0)<800) //this is the condition to be checked
  {
   // if button is pressed - throw the dice
  pressed = digitalRead(button);

  if (pressed == HIGH) {
    // remove previous number
    setAllLEDs(LOW);
    
    buildUpTension();
    int thrownNumber = throwDice();
    showNumber(thrownNumber);
    }
  } 



}