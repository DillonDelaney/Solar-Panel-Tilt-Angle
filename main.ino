#include <EEPROM.h>
// EEPROM addresses
const int calibDateAddress = 0; // Address to store the calibration date
const int elapsedDaysAddress = 2; // Address to store elapsed days since calibration
int calibrationDay = 1; // Day of the year when calibration was done
int elapsedDays = 0; // Days elapsed since calibration

// Write these values to EEPROM only once, manually uncomment these lines to set or reset the calibration
//writeIntToEEPROM(calibDateAddress, calibrationDay);
//writeIntToEEPROM(elapsedDaysAddress, elapsedDays);

// Variables for timing and direction
const int totalDays = 365;
unsigned long lastUpdateTime = 0; // Store the last update time in milliseconds
const unsigned long dayDuration = 86400000; // 24 hours in milliseconds for simulation

// Actuator extension values for half year
//First Claebration has to be the extension of 13.1mm from start
const float actuatorExtensions[183] = {0, 
0.745434505272101, 
0.764841036454982, 
0.784135655221348, 
0.803316151598892, 
0.822380373264764, 
0.841326225963485, 
0.860151673867449, 
0.878854739869894, 
0.897433505820686, 
0.915886112697649, 
0.934210760723431, 
0.95240570941894, 
0.970469277604707, 
0.988399843343473, 
1.00619584383048, 
1.0238557752308, 
1.041378192465, 
1.05876170894624, 
1.07600499626892, 
1.09310678385179, 
1.11006585853625, 
1.12688106414089, 
1.14355130097772, 
1.16007552532687, 
1.17645274887434, 
1.19268203811566, 
1.20876251372397, 
1.22469334988591, 
1.2404737736108, 
1.25610306400711, 
1.27158055153438, 
1.28690561723113, 
1.30207769191804, 
1.31709625538122, 
1.33196083553435, 
1.34667100756508, 
1.36122639306257, 
1.37562665913299, 
1.38987151749848, 
1.40396072358908, 
1.41789407562032, 
1.43167141366473, 
1.44529261871537, 
1.45875761174307, 
1.47206635275063, 
1.48521883982175, 
1.49821510817003, 
1.5110552291859, 
1.52373930948363, 
1.53626748995157, 
1.54863994480263, 
1.56085688063013, 
1.57291853546678, 
1.58482517785023, 
1.59657710589295, 
1.60817464636204, 
1.61961815376344, 
1.63090800943803, 
1.64204462066425, 
1.65302841977154, 
1.6638598632652, 
1.67453943096064, 
1.68506762513005, 
1.69544496966068, 
1.70567200922596, 
1.7157493084693, 
1.72567745120011, 
1.73545703960485, 
1.74508869347108, 
1.75457304942552, 
1.76391076018569, 
1.77310249382856, 
1.78214893306995, 
1.79105077456234, 
1.79980872820568, 
1.80842351647266, 
1.81689587375138, 
1.82522654570027, 
1.83341628862013, 
1.84146586884026, 
1.84937606211997, 
1.85714765306511, 
1.8647814345602, 
1.87227820721284, 
1.87963877881748, 
1.88686396382889, 
1.89395458285401, 
1.90091146215639, 
1.90773543317459, 
1.9144273320569, 
1.92098799920677, 
1.92741827884515, 
1.93371901858461, 
1.93989106901677, 
1.9459352833145, 
1.95185251684575, 
1.95764362679961, 
1.96330947182787, 
1.96885091169463, 
1.9742688069424, 
1.97956401856686, 
1.98473740770498, 
1.98978983533368, 
1.99472216198092, 
1.99953524744603, 
2.00422995053248, 
2.00880712879007, 
2.01326763826773, 
2.01761233327602, 
2.02184206616062, 
2.02595768708306, 
2.0299600438135, 
2.03384998153035, 
2.03762834263057, 
2.04129596654627, 
2.04485368957285, 
2.0483023447016, 
2.05164276146434, 
2.05487576578196, 
2.05800217982301, 
2.06102282186805, 
2.06393850618315, 
2.06675004289707, 
2.06945823788828, 
2.07206389267662, 
2.07456780432119, 
2.07697076532622, 
2.07927356355003, 
2.0814769821211, 
2.08358179936135, 
2.08558878871054, 
2.08749871866053, 
2.08931235269074, 
2.09103044921096, 
2.09265376150711, 
2.0941830376936, 
2.09561902066628, 
2.09696244806435, 
2.09821405223227, 
2.09937456018781, 
2.10044469359332, 
2.10142516872975, 
2.10231669647544, 
2.10311998228735, 
2.10383572618605, 
2.10446462274274, 
2.1050073610711, 
2.10546462481966, 
2.10583709216894, 
2.10612543582999, 
2.10633032304543, 
2.10645241559394, 
2.10649236979555, 
2.10645083652025, 
2.10632846119802, 
2.10612588383208, 
2.10584373901088, 
2.1054826559274, 
2.10504325839315, 
2.10452616486134, 
2.10393198844451, 
2.10326133693911, 
2.10251481284888, 
2.10169301340966, 
2.10079653061621, 
2.09982595124927, 
2.09878185690627, 
2.09766482402836, 
2.09647542393384, 
2.09521422284888, 
2.09388178194001, 
2.09247865734915, 
2.09100540022598, 
2.08946255676483, 
2.08785066823998, 
2.0861702710414, 
2.08442189671337, 
2.0826060719902, 
2.08072331883602, 
2.07877415448274, 
2.07675909146872, 
2.07467863767909};

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
    Serial.println(extension);
    elapsedDays++;
    //writeIntToEEPROM(elapsedDaysAddress, elapsedDays);
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
    analogWrite(RPWM,255); //BTS Code
    analogWrite(LPWM,0); // BTS Code
  } else {
    analogWrite(RPWM,0); //BTS Code
    analogWrite(LPWM,255); // BTS Code
  }

  delay(extension);

  analogWrite(LPWM,0); //BTS Code
  analogWrite(RPWM,0); //BTS Code
}
