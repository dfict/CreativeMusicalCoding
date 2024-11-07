//test all ins to make sure your board is working after you built it.
// look at serial monitor (FTDI and make sure inputs work)

const int NUM_POTS = 6;
const int NUM_BUTTONS = 4;

const int potPins[NUM_POTS] = {A0, A1, A2, A3, A4, A5};
const int buttonPins[NUM_BUTTONS] = {2, 3, 4, 5};

void setup() {
  Serial.begin(9600);
  
  for (int i = 0; i < NUM_BUTTONS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
  }
}

void loop() {
  // Read and print potentiometer values
  for (int i = 0; i < NUM_POTS; i++) {
    int potValue = analogRead(potPins[i]);
    Serial.print("P");
    Serial.print(i + 1);
    Serial.print(":");
    Serial.print(potValue);
    Serial.print("  ");
  }
  
  // Read and print button states
  for (int i = 0; i < NUM_BUTTONS; i++) {
    int buttonState = digitalRead(buttonPins[i]);
    Serial.print("B");
    Serial.print(i + 1);
    Serial.print(":");
    Serial.print(buttonState == LOW ? "1" : "0");
    Serial.print("  ");
  }
  
  Serial.println(); // Move to the next line
  delay(20); // Small delay to avoid flooding the serial monitor
}
