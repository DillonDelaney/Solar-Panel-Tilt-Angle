#include <Wire.h> // Include Wire library for I2C communication (needed for RTC)
#include <RTClib.h> // Include RTC library

// Relay control pins
const int relayPin1 = 2; // Controls the motor direction 1
const int relayPin2 = 3; // Controls the motor direction 2

// Variables for timing and direction
const int totalDays = 365; // Adjusted for a full year cycle
DateTime lastUpdateTime; // Store the last update time

RTC_DS3231 rtc; // Create an RTC object

// Actuator extension values for half year, simulating the first half cycle
const float actuatorExtensions[182] = {
    13.188586, 0.745434505, 0.764841036, 0.784135655, 0.803316152, 0.822380373, 0.841326226, 0.860151674, 0.87885474,
    // The rest of the values until 2.074678638
};

void setup() {
  // Initialize the relay pins as outputs and serial communication
  pinMode(relayPin1, OUTPUT);
  pinMode(relayPin2, OUTPUT);
  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
  Serial.begin(9600);

  // Initialize the RTC module
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); // Stop execution if RTC is not found
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Load the last update time from RTC
  lastUpdateTime = rtc.now();
}

void loop() {
  DateTime now = rtc.now(); // Get current time from RTC
  long daysElapsed = (now - lastUpdateTime).days();

  if (daysElapsed >= 1) {
    lastUpdateTime = now; // Update the last update time

    int currentDayOfYear = lastUpdateTime.dayOfYear();
    bool direction;
    float extension;

    if (currentDayOfYear <= 182) {
      direction = true; // Increasing tilt for the first half of the year
      extension = actuatorExtensions[currentDayOfYear - 1];
    } else if (currentDayOfYear > 182 && currentDayOfYear <= 364) {
      direction = false; // Decreasing tilt for the second half of the year
      int reverseIndex = 364 - currentDayOfYear;
      extension = actuatorExtensions[reverseIndex];
    } else {
      // Handle the extra day in a leap year or reset at the end of the year
      direction = false;
      extension = actuatorExtensions[0]; // Reset or adjust as needed
    }

    // Move the motor in the calculated direction
    moveMotor(direction);

    // Debugging: print the current day, direction, and extension
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

  // Example duration based on extension
  unsigned long duration = 5000; // Example: 5 seconds
  delay(duration);

  digitalWrite(relayPin1, LOW);
  digitalWrite(relayPin2, LOW);
}
