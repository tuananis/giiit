#include <SD.h>
#include <SPI.h>
#include <Wire.h> 
#include "RTClib.h"
#include <Keypad.h>
#include <LiquidCrystal_I2C.h>

#if defined(ARDUINO_ARCH_SAMD)
  // for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

File myFile;
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTC_DS1307 rtc;

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns

char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'S','0','E','D'}
};

byte colPins[COLS] = {28,26,24,22}; //connect to the column pinouts of the keypad
byte rowPins[ROWS] = {36,34,32,30}; //connect to the row pinouts of the keypad

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); 

int pinCS = 53; // Pin 10 on Arduino Uno

String v_date;
String v_time;

void setup() {
  
  Serial.begin(9600);

  //LCD      
  lcd.begin();      
  lcd.backlight();
  lcd.clear();
  lcd.noCursor();
     
  lcd.setCursor(0,0); lcd.print(" Wearable Body Fat");
  lcd.setCursor(0,1); lcd.print("     Calculator");
  lcd.setCursor(0,2); lcd.print("*** Developed By ***");  
  lcd.setCursor(0,3); lcd.print("Dinesh Kaushal- PIBT");   
  delay(2000);
  lcd.clear();
  
  //Real Time Clock
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

  //SD Card
  pinMode(pinCS, OUTPUT);  
  // SD Card Initialization
  if (SD.begin())
  {
    Serial.println("SD card is ready to use.");
  } else
  {
    Serial.println("SD card initialization failed");
    return;
  }
    
}

void loop() {
  
  DateTime now = rtc.now();
  
  v_date=String(now.year(), DEC);
  v_date=v_date + "-" + String(now.month(), DEC);
  v_date=v_date + "-" + String(now.day(), DEC);

  v_time=String(now.hour(), DEC);
  v_time=v_time + ":" + String(now.minute(), DEC);
  v_time=v_time + ":" + String(now.second(), DEC);

  lcd.setCursor(5,0); lcd.print("               ");  
  lcd.setCursor(5,1); lcd.print("               ");  
  
  lcd.setCursor(0,0); lcd.print("Date:"); lcd.setCursor(5,0); lcd.print(v_date);  
  lcd.setCursor(0,1); lcd.print("Time:"); lcd.setCursor(5,1); lcd.print(v_time);  
  
  char customKey = customKeypad.getKey();
  if (customKey != NO_KEY)
  {
    Serial.println(customKey);
    
    if(customKey=='A')
    {    
      // Create/Open file 
      myFile = SD.open("record.txt", FILE_WRITE);
  
      // if the file opened okay, write to it:
      if (myFile) {
        Serial.println("Writing to file...");
        // Write to file
        myFile.println(v_date + "," + v_time);
        myFile.close(); // close the file
        Serial.println("Done.");
      }
      // if the file didn't open, print an error:
      else {
        Serial.println("error opening test.txt");
      }  
    }
  }
  //delay(1000);
}


  /*
  // Reading the file
  myFile = SD.open("test.txt");
  if (myFile) {
    Serial.println("Read:");
    // Reading the whole file
    while (myFile.available()) {
      Serial.write(myFile.read());
   }
    myFile.close();
  }
  else {
    Serial.println("error opening test.txt");
  }
  */

  
