# include "pitches.h"


// Pin assignments for the shift register and buzzer
int latchPin = 8;
int clockPin = 12;
int dataPin = 11;
int buzzerPin = 9;

// Holders for data sent to the shift register
byte data;
byte dataArray[10];

// Melody for Happy Birthday
int melody[] = {
  NOTE_C4, NOTE_C4, 
  NOTE_D4, NOTE_C4, NOTE_F4,
  NOTE_E4, NOTE_C4, NOTE_C4, 
  NOTE_D4, NOTE_C4, NOTE_G4,
  NOTE_F4, NOTE_C4, NOTE_C4,
  
  NOTE_C5, NOTE_A4, NOTE_F4, 
  NOTE_E4, NOTE_D4, NOTE_AS4, NOTE_AS4,
  NOTE_A4, NOTE_F4, NOTE_G4,
  NOTE_F4
};

int noteDurations[] = {
  4, 8, 
  4, 4, 4,
  2, 4, 8, 
  4, 4, 4,
  2, 4, 8,
  
  4, 4, 4, 
  4, 4, 4, 8,
  4, 4, 4,
  2
};

unsigned long lastShiftTime = 0;
unsigned long shiftDelay = 300;
int j = 0;

void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);

  Serial.begin(9600);

  // Initialize the dataArray for LED control
  dataArray[0] = 0xFF; 
  dataArray[1] = 0xFE;
  dataArray[2] = 0xFC;
  dataArray[3] = 0xF8;
  dataArray[4] = 0xF0;
  dataArray[5] = 0xE0;
  dataArray[6] = 0xC0;
  dataArray[7] = 0x80;
  dataArray[8] = 0x00;
  dataArray[9] = 0xE0;

  // Play Happy Birthday melody immediately
  playHappyBirthday();
}

void loop() {
  // Update shift register with the light sequence
  if (millis() - lastShiftTime >= shiftDelay) {
    data = dataArray[j];
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, data);
    digitalWrite(latchPin, HIGH);
    lastShiftTime = millis();
    j++;
    if (j >= 10) j = 0; // Loop back to the first data
  }
}

// Function to shift data out to the shift register
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i = 0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);
  digitalWrite(myDataPin, LOW);
  digitalWrite(myClockPin, LOW);

  for (i = 7; i >= 0; i--) {
    digitalWrite(myClockPin, LOW);
    pinState = (myDataOut & (1 << i)) ? HIGH : LOW;
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, HIGH);
    digitalWrite(myDataPin, LOW);
  }
  digitalWrite(myClockPin, LOW);
}

// Function to blink all LEDs (used at the beginning)
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, HIGH);
  delay(200);

  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, HIGH);
    delay(d);
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, HIGH);
    delay(d);
  }
}

// Function to play Happy Birthday melody
void playHappyBirthday() {
  for (int thisNote = 0; thisNote < 32; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}
