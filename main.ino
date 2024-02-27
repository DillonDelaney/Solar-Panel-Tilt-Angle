

// Relay control pins
const int relayPin1 = 4;
const int relayPin2 = 5;

// Variables for timing and direction
const int totalDays = 365;
unsigned long lastUpdateTime = 0; // Store the last update time in milliseconds
const unsigned long dayDuration = 86400000; // 24 hours in milliseconds for simulation

// Actuator extension values for half year
const float actuatorExtensions[182] = {
    // Include all your actuator extension values here
};

void setup() {
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  Serial.begin(9600);

  // Set the initial last update time
  lastUpdateTime = millis();
}

void loop() {
  unsigned long now = millis(); // Get current time in milliseconds
  unsigned long timeElapsed = now - lastUpdateTime;

  if (timeElapsed >= dayDuration) { // Simulate a day has passed
    lastUpdateTime = now; // Update the last update time

    // Simulate day of the year based on elapsed time
    // Note: This is a simplified simulation and might need adjustments for accurate day/year calculations
    int currentDayOfYear = (timeElapsed / dayDuration) % 365 + 1;
    bool direction;
    float extension;

    if (currentDayOfYear <= 182) {
      direction = true;
      extension = actuatorExtensions[currentDayOfYear - 1];
    } else if (currentDayOfYear > 182 && currentDayOfYear <= 364) {
      direction = false;
      int reverseIndex = 364 - currentDayOfYear;
      extension = actuatorExtensions[reverseIndex];
    } else {
      direction = false;
      extension = actuatorExtensions[0];
    }

    moveMotor(direction);

    Serial.print("Day ");
    Serial.print(currentDayOfYear);
    Serial.print(": Direction ");
    Serial.print(direction ? "Increasing" : "Decreasing");
    Serial.print(", Extension: ");
    Serial.println(extension);
  }
}

void moveMotor(bool direction) {
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);

  if (direction) {
    digitalWrite(relayPin1, HIGH);
  } else {
    digitalWrite(relayPin2, HIGH);
  }

  unsigned long duration = 5000; // Example duration
  delay(duration);

  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
}
