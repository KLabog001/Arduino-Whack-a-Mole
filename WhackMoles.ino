// 7 Segment Output
const int SevensegPins[7] = {2,3,4,5,6,7,8}; // define pins
byte displayLEDs[10][7] = { 
        { 0,0,0,0,0,0,1 },  // = 0 = 0x01
        { 1,0,0,1,1,1,1 },  // = 1 = 0x4F
        { 0,0,1,0,0,1,0 },  // = 2 = 0x12
        { 0,0,0,0,1,1,0 },  // = 3 = 0x06
        { 1,0,0,1,1,0,0 },  // = 4 = 0x4C
        { 0,1,0,0,1,0,0 },  // = 5 = 0x24
        { 0,1,0,0,0,0,0 },  // = 6 = 0x20
        { 0,0,0,1,1,1,1 },  // = 7 = 0x0F
        { 0,0,0,0,0,0,0 },  // = 8 = 0x00
        { 0,0,0,0,1,0,0 }   // = 9 =0X04
        }; 

// buttoninput variables
int red = 0;
int blue = 0;
int green = 0;

//pin mapping
int redButton = 10;
int redLED = 12;

int blueButton = 9;
int blueLED = 11;

int greenButton = A0;
int greenLED = A1; 

//flags and control signals
int mole = 0;
int score = 0;
int maxScore = 9;
int gameWon = 1;
int resetButtonPressed = 0;
int updatedScore = 0;  // flag
int switchMole = 1;

//timing variables
unsigned long ledOnStartTime = 0;
unsigned long MaxLEDTime = 1500;
unsigned long lastStageLedTime = 400;

void setup()
{
  
  Serial.begin(9600);
    for(int d=0; d<8; d++)
  {
    pinMode(SevensegPins[d], OUTPUT);// set pins as output 
  }
  displayScore(score);
  pinMode(redLED, OUTPUT);  
  pinMode(blueLED, OUTPUT); 
  pinMode(greenLED, OUTPUT);
  pinMode(redButton, INPUT); 
  pinMode(blueButton, INPUT); 
  pinMode(greenButton,INPUT);
}

void displayScore(int number) {
  for (int i=0; i < 7; i++) 
  {
      digitalWrite(SevensegPins[i], displayLEDs[number][i]);// 
  }
}


// Manage Timing
void updateLEDTiming() {  
  unsigned long ledTimeOn = millis() - ledOnStartTime;
  if (ledTimeOn >= MaxLEDTime || updatedScore == 1) {
    Serial.print("Switch Led when time was: "); 
    Serial.print(ledTimeOn, DEC); 
    updatedScore = 0;
    digitalWrite(blueLED, LOW);
    digitalWrite(redLED, LOW);
    digitalWrite(greenLED, LOW);
    delay(300);
    switchMole = 1;
    displayScore(score);
  }
}

//Manage Speed
void updateLEDSpeedTiming() {  
  if (score < 3) {
    MaxLEDTime = 1200;
  } else if (score < 6) {
    MaxLEDTime = 800;
  } else if (score > 6) {
    MaxLEDTime = 400;
  } 
}

// Choose Mole
void displayMole() {
  if (switchMole == 1) {
    mole = rand() % 3 + 1;
    ledOnStartTime = millis();
    if (mole == 1) {
      Serial.print("RED MOLE");
      Serial.print("\n");
      digitalWrite(redLED, HIGH);
      digitalWrite(blueLED, LOW);
      digitalWrite(greenLED, LOW);
    } else if (mole == 2) {
      Serial.print("BLUE MOLE");
      Serial.print("\n");
      digitalWrite(blueLED, HIGH);
      digitalWrite(redLED, LOW);
      digitalWrite(greenLED, LOW);
    } else if (mole == 3) {
      Serial.print("GREEN MOLE");
      Serial.print("\n");
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
      digitalWrite(blueLED, LOW);
    }
    switchMole = 0;
  }
}

// Score Management
void whackMole() {
  red = digitalRead(redButton);  // read input value
  blue = digitalRead(blueButton); // read input value
  green = digitalRead(greenButton); // read input value
  
  if (red == HIGH) { // check if redButton is pressed
    Serial.print("Red Button Pressed");
    Serial.print("\n");  
    updatedScore = 1;     
    if (mole == 1 && score < maxScore) {
      score += 1;
    } else if (score > 0) {
      score -= 1;
    } 
  }
  if (blue == HIGH) {  // read input value 
    Serial.print("Blue Button Pressed");
    Serial.print("\n");
    updatedScore = 1;
    if (mole == 2 && score < maxScore) {
      score += 1;
    } else if (score > 0) {
      score -= 1;
    } 
  }
  if (green == HIGH) {  // read input value 
    Serial.print("Green Button Pressed");
    Serial.print("\n");
    updatedScore = 1;
    if (mole == 3 && score < maxScore) {
      score += 1;
    } else if (score > 0) {
      score -= 1;
    } 
  }
}

void loop() {
  red = digitalRead(redButton);
  if (gameWon == 1 && red == 1) { // reset score on press
      score = 0; // Reset score
      resetButtonPressed = 1;
  } else if (gameWon == 1 && red == 0 && resetButtonPressed == 1) {  // reset game on depress
    gameWon = 0; // Reset game
  }
  else if (gameWon == 0 && score < maxScore) {
    updateLEDTiming();
    displayMole();
    whackMole();
    updateLEDSpeedTiming();
  } else if (score == maxScore) {
    
    gameWon = 1;
    displayScore(score);
    digitalWrite(redLED, HIGH);
    digitalWrite(blueLED, HIGH);
    digitalWrite(greenLED, HIGH);
    delay(200);
  }
}
