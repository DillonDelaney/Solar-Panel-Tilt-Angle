#include <EEPROM.h>

#define RPWM 3 // BTS Code
#define LPWM 6 // BTS Code
#define REN 2 // BTS Code
#define LEN 5 // BTS Code
// EEPROM addresses
const int calibDateAddress = 0; // Address to store the calibration date
const int elapsedDaysAddress = 2; // Address to store elapsed days since calibration
int calibrationDay = 1; // Day of the year when calibration was done
int elapsedDays = 0; // Days elapsed since calibration



// Variables for timing and direction
const int totalDays = 365;
unsigned long lastUpdateTime = 0; // Store the last update time in milliseconds
const unsigned long dayDuration = 5000; // 24 hours in milliseconds for simulation

// Actuator extension values for half year
//First Claebration has to be the extension of 11.7seonds from start
const float actuatorExtensions[183] = {0,
0.236648024992451,
0.237385047510679,
0.23811350422709,
0.23883343655657,
0.23954488608656,
0.240247894563936,
0.240942503882979,
0.241628756072623,
0.242306693284718,
0.242976357782482,
0.243637791928859,
0.244291038175516,
0.244936139051947,
0.245573137154952,
0.246202075138149,
0.2468229957022,
0.247435941584672,
0.248040955550687,
0.248638080383671,
0.249227358876078,
0.249808833820794,
0.250382548002494,
0.25094854418935,
0.251506865124838,
0.252057553519899,
0.252600652045453,
0.253136203324857,
0.253664249926555,
0.25418483435742,
0.254697999055725,
0.255203786384777,
0.255702238626282,
0.256193397974494,
0.256677306530037,
0.25715400629424,
0.257623539163501,
0.258085946923939,
0.258541271246168,
0.258989553680181,
0.25943083565071,
0.259865158452249,
0.260292563244752,
0.260713091049251,
0.261126782743554,
0.261533679058334,
0.261933820573154,
0.262327247712847,
0.262714000743836,
0.263094119770744,
0.263467644733123,
0.263834615402288,
0.264195071378208,
0.264549052086818,
0.264896596776975,
0.265237744518186,
0.265572534197804,
0.265901004518719,
0.266223193997166,
0.266539140960572,
0.266848883545322,
0.267152459695058,
0.267449907158721,
0.26774126348887,
0.268026566040021,
0.268305851967239,
0.268579158224429,
0.268846521563387,
0.269107978532247,
0.269363565474441,
0.269613318527604,
0.269857273622642,
0.270095466482758,
0.270327932622672,
0.270554707347787,
0.27077582575364,
0.270991322725217,
0.271201232936414,
0.271405590849599,
0.27160443071532,
0.271797786571688,
0.271985692244402,
0.272168181346433,
0.272345287277744,
0.272517043225503,
0.272683482163711,
0.272844636853415,
0.273000539842724,
0.273151223466948,
0.273296719848759,
0.273437060898323,
0.273572278313691,
0.273702403581024,
0.273827467975025,
0.273947502559054,
0.27406253818599,
0.27417260549837,
0.274277734928933,
0.274377956701301,
0.274473300830346,
0.274563797123028,
0.27464947517883,
0.274730364390552,
0.27480649394488,
0.274877892823292,
0.274944589802658,
0.275006613456048,
0.275063992153628,
0.275116754063333,
0.275164927151832,
0.275208539185405,
0.275247617730793,
0.275282190155986,
0.275312283631547,
0.275337925131066,
0.275359141432477,
0.275375959118901,
0.275388404579758,
0.275396504011587,
0.275400283419322,
0.275399768617081,
0.275394985229468,
0.275385958692482,
0.275372714254627,
0.275355276978053,
0.275333671739668,
0.275307923232094,
0.275278055964964,
0.275244094266077,
0.275206062282314,
0.275163983980974,
0.27511788315087,
0.275067783403483,
0.275013708174083,
0.274955680723042,
0.274893724136746,
0.274827861329095,
0.274758115042346,
0.274684507848656,
0.274607062150881,
0.274525800184119,
0.274440744016647,
0.274351915551256,
0.274259336526393,
0.274163028517341,
0.274063012937449,
0.273959311039357,
0.273851943916043,
0.273740932502233,
0.273626297575424,
0.27350805975716,
0.27338623951423,
0.273260857159721,
0.273131932854517,
0.272999486608114,
0.272863538280035,
0.272724107580984,
0.272581214073989,
0.272434877175533,
0.2722851161569,
0.272131950145081,
0.271975398124187,
0.271815478936537,
0.271652211283663,
0.271485613727737,
0.271315704692483,
0.271142502464431,
0.270966025194184,
0.270786290897104,
0.270603317455152,
0.2704171226173,
0.270227724001165,
0.270035139093807,
0.269839385253067,
0.269640479708528,
0.269438439562691,
0.269233281792003,
0.269025023247991,
0.268813680658354,
0.26859927062801,
0.268381809640209,
0.268161314057579,
0.0289478853019546}
;

void setup() {

  pinMode(RPWM,OUTPUT);//BTS Code
  pinMode(LPWM,OUTPUT); //BTS Code
  
  pinMode(LEN,OUTPUT); //BTS Code
  pinMode(REN,OUTPUT); //BTS Code
  
  digitalWrite(REN,HIGH);//BTS Code
  digitalWrite(LEN,HIGH); //BTS Code

  Serial.begin(9600);

  // Set the initial last update time
  lastUpdateTime = millis();
  // Write these values to EEPROM only once, manually uncomment these lines to set or reset the calibration
//writeIntToEEPROM(calibDateAddress, calibrationDay);
//writeIntToEEPROM(elapsedDaysAddress, elapsedDays);
}

void loop() {
  // Read the calibration date and elapsed days from EEPROM
  int calibrationDay = readIntFromEEPROM(calibDateAddress);
  int elapsedDays = readIntFromEEPROM(elapsedDaysAddress);
  
  unsigned long now = millis(); // Get current time in milliseconds
  unsigned long timeElapsed = now - lastUpdateTime;

  if (timeElapsed >= dayDuration) { // Simulate a day has passed
    lastUpdateTime = now; // Update the last update time

    // Simulate day of the year based on elapsed time
    // Calculate the current day based on the calibration date and elapsed days
    int currentDayOfYear = (calibrationDay + elapsedDays) % 365; // Simple calculation, adjust for leap years as needed
    bool direction;
    float extension;

    if (currentDayOfYear <= 183) {
      direction = true;
      extension = actuatorExtensions[currentDayOfYear - 1];
    } else if (currentDayOfYear > 183 && currentDayOfYear <= 366) {
      direction = false;
      int reverseIndex = 366 - currentDayOfYear;
      extension = actuatorExtensions[reverseIndex];
    } else {
      direction = false;
      extension = actuatorExtensions[0];
    }

    moveMotor(direction, extension);
    
    Serial.print("Day ");
    Serial.print(currentDayOfYear);
    Serial.print(": Direction ");
    Serial.print(direction ? "Increasing" : "Decreasing");
    Serial.print(", Extension: ");
    Serial.println(extension*1000);
    elapsedDays++;
    writeIntToEEPROM(elapsedDaysAddress, elapsedDays);
  }
}

// Function to write an integer (e.g., day of the year) to EEPROM at a specific address
void writeIntToEEPROM(int address, int value) {
  byte lowByte = value & 0xFF; // Extract the low byte
  byte highByte = (value >> 8) & 0xFF; // Extract the high byte
  EEPROM.update(address, lowByte); // Write the low byte (minimizes unnecessary writes)
  EEPROM.update(address + 1, highByte); // Write the high byte
}
// Function to read an integer (e.g., day of the year) from EEPROM at a specific address
int readIntFromEEPROM(int address) {
  byte lowByte = EEPROM.read(address);
  byte highByte = EEPROM.read(address + 1);
  return (int)lowByte | ((int)highByte << 8);
}

void moveMotor(bool direction, float extension) {
  analogWrite(LPWM,0); //BTS Code
  analogWrite(RPWM,0); //BTS Code

  if (direction) {
    analogWrite(RPWM,0); //BTS Code
    analogWrite(LPWM,255); // BTS Code
  } else {
    analogWrite(RPWM,255); //BTS Code
    analogWrite(LPWM,0); // BTS Code
  }

  delay(extension*1000);

  analogWrite(LPWM,0); //BTS Code
  analogWrite(RPWM,0); //BTS Code
}
