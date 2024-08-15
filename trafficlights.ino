#include <avr/wdt.h>

int REDPIN1 = 9;
int YELLOWPIN1 = 11;
int GREENPIN1 = 8;

int REDPIN2 = 12;
int YELLOWPIN2 = 10;
int GREENPIN2 = 13;

int BUTTONPIN = 2;

unsigned long previousMillis = 0;  // Stores the last time the state was updated
const long greenDuration = 2500;   // Duration for green light (5 seconds)
const long yellowDuration = 750;  // Duration for yellow light (1.5 seconds)
const long initialRedDuration = 2500; // Initial duration for red light (5 seconds)
const long shortRedDuration = 500;   // Shortened duration for red light (1 second)
const long redAndYellowDuration = 1500; // Duration for red and yellow light (3 seconds)

int currentState = 0; // Variable to keep track of the current state
bool buttonPressed = false; // To track the button press
bool buttonReleased = true; // To track if the button was released after pressing
bool sequenceStarted = false; // To track if the sequence has started
bool firstCycle = true; // To track if it's the first cycle
bool isRunning = false; // To track whether the sequence is running or stopped

void setup() {
  // Set the Watchdog Timer to reset after 2 seconds if not cleared
  wdt_enable(WDTO_2S);

  // Set the LED pins to output pins
  pinMode(REDPIN1, OUTPUT);
  pinMode(YELLOWPIN1, OUTPUT);
  pinMode(GREENPIN1, OUTPUT);

  pinMode(REDPIN2, OUTPUT);
  pinMode(YELLOWPIN2, OUTPUT);
  pinMode(GREENPIN2, OUTPUT);

  // Set the button pin to input with pull-up resistor
  pinMode(BUTTONPIN, INPUT_PULLUP);

  // Initialize with all lights red
  resetLights();
}

void loop() {
  wdt_reset();  // Clear the watchdog timer

  unsigned long currentMillis = millis();

  // Existing logic...
  // Read the button state
  if (digitalRead(BUTTONPIN) == LOW) {
    if (buttonReleased) { // Ensure the button was released before detecting a new press
      buttonPressed = true;
      buttonReleased = false;

      if (sequenceStarted) {
        // Reset the system if already started
        resetLights();
      } else {
        // Start the sequence
        sequenceStarted = true;
        isRunning = true;
        currentState = 0; // Start from the beginning (Red light for Direction 1)
        previousMillis = currentMillis; // Reset the timer
      }
    }
  } else {
    // Button is not pressed
    buttonReleased = true; // Set to true to allow a new press to be detected
  }

  if (sequenceStarted && isRunning) {
    long currentRedDuration = firstCycle ? initialRedDuration : shortRedDuration;

    // Control the lights based on the current state
    if (currentState == 0) { // Red Light for Direction 1
      if (currentMillis - previousMillis >= currentRedDuration) {
        previousMillis = currentMillis;
        currentState = 1; // Move to the next state (Red and Yellow for Direction 1)
      }
      // Direction 1 is red
      digitalWrite(REDPIN1, HIGH);
      digitalWrite(YELLOWPIN1, LOW);
      digitalWrite(GREENPIN1, LOW);

      digitalWrite(REDPIN2, HIGH);
      digitalWrite(YELLOWPIN2, LOW);
      digitalWrite(GREENPIN2, LOW);
    } else if (currentState == 1) { // Red and Yellow Light for Direction 1
      if (currentMillis - previousMillis >= redAndYellowDuration) {
        previousMillis = currentMillis;
        currentState = 2; // Move to the next state (Green for Direction 1)
      }
      // Direction 1 is red and yellow
      digitalWrite(REDPIN1, HIGH);
      digitalWrite(YELLOWPIN1, HIGH);
      digitalWrite(GREENPIN1, LOW);

      digitalWrite(REDPIN2, HIGH);
      digitalWrite(YELLOWPIN2, LOW);
      digitalWrite(GREENPIN2, LOW);
    } else if (currentState == 2) { // Green Light for Direction 1
      if (currentMillis - previousMillis >= greenDuration) {
        previousMillis = currentMillis;
        currentState = 3; // Move to the next state (Yellow for Direction 1)
      }
      // Direction 1 is green
      digitalWrite(REDPIN1, LOW);
      digitalWrite(YELLOWPIN1, LOW);
      digitalWrite(GREENPIN1, HIGH);

      digitalWrite(REDPIN2, HIGH);
      digitalWrite(YELLOWPIN2, LOW);
      digitalWrite(GREENPIN2, LOW);
    } else if (currentState == 3) { // Yellow Light for Direction 1 (before Red)
      if (currentMillis - previousMillis >= yellowDuration) {
        previousMillis = currentMillis;
        currentState = 4; // Move to the next state (Red Light for Direction 2)
      }
      // Direction 1 is yellow
      digitalWrite(REDPIN1, LOW);
      digitalWrite(YELLOWPIN1, HIGH);
      digitalWrite(GREENPIN1, LOW);

      digitalWrite(REDPIN2, HIGH);
      digitalWrite(YELLOWPIN2, LOW);
      digitalWrite(GREENPIN2, LOW);
    } else if (currentState == 4) { // Red Light for Direction 2
      if (currentMillis - previousMillis >= currentRedDuration) {
        previousMillis = currentMillis;
        currentState = 5; // Move to the next state (Red and Yellow for Direction 2)
      }
      // Direction 2 is red
      digitalWrite(REDPIN1, HIGH);
      digitalWrite(YELLOWPIN1, LOW);
      digitalWrite(GREENPIN1, LOW);

      digitalWrite(REDPIN2, HIGH);
      digitalWrite(YELLOWPIN2, LOW);
      digitalWrite(GREENPIN2, LOW);
    } else if (currentState == 5) { // Red and Yellow Light for Direction 2
      if (currentMillis - previousMillis >= redAndYellowDuration) {
        previousMillis = currentMillis;
        currentState = 6; // Move to the next state (Green for Direction 2)
      }
      // Direction 2 is red and yellow
      digitalWrite(REDPIN1, HIGH);
      digitalWrite(YELLOWPIN1, LOW);
      digitalWrite(GREENPIN1, LOW);

      digitalWrite(REDPIN2, HIGH);
      digitalWrite(YELLOWPIN2, HIGH);
      digitalWrite(GREENPIN2, LOW);
    } else if (currentState == 6) { // Green Light for Direction 2
      if (currentMillis - previousMillis >= greenDuration) {
        previousMillis = currentMillis;
        currentState = 7; // Move to the next state (Yellow for Direction 2)
      }
      // Direction 2 is green
      digitalWrite(REDPIN1, HIGH);
      digitalWrite(YELLOWPIN1, LOW);
      digitalWrite(GREENPIN1, LOW);

      digitalWrite(REDPIN2, LOW);
      digitalWrite(YELLOWPIN2, LOW);
      digitalWrite(GREENPIN2, HIGH);
    } else if (currentState == 7) { // Yellow Light for Direction 2 (before Red)
      if (currentMillis - previousMillis >= yellowDuration) {
        previousMillis = currentMillis;
        currentState = 0; // Move to the next state (Red Light for Direction 1)
        firstCycle = false; // Indicate that the first cycle is complete
      }
      // Direction 2 is yellow
      digitalWrite(REDPIN1, HIGH);
      digitalWrite(YELLOWPIN1, LOW);
      digitalWrite(GREENPIN1, LOW);

      digitalWrite(REDPIN2, LOW);
      digitalWrite(YELLOWPIN2, HIGH);
      digitalWrite(GREENPIN2, LOW);
    }
  }
}

void resetLights() {
  // Reset all lights to red
  digitalWrite(REDPIN1, HIGH);
  digitalWrite(YELLOWPIN1, LOW);
  digitalWrite(GREENPIN1, LOW);

  digitalWrite(REDPIN2, HIGH);
  digitalWrite(YELLOWPIN2, LOW);
  digitalWrite(GREENPIN2, LOW);

  // Reset the state variables
  currentState = 0;
  sequenceStarted = false;
  isRunning = false;
  firstCycle = true;
}
