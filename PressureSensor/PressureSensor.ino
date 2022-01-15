#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include <string.h>

// Analog Pins
int analog1, analog2 = A1, A2;
int fpressure, rpressure;

// Digital Pins (for recording)
int digital0 = 1;
int recording, last_recording == LOW, LOW;

// File
char filename[12];
File myFile;

// LCD
// set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27,16,2);
DS3231  rtc(SDA, SCL);

void setup()
{
  Serial.begin(9600);
  rtc.begin();
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  myFile.close();

  // Clock calibration
  //rtc.setDOW(THURSDAY);
  //rtc.setTime(1,21,00);
  //rtc.setDate(11, 11, 2021);
}

void print_Serial(int fpressure, int rpressure){
    Serial.println("FPres: " + (String)fpressure);
    Serial.println("RPres: " + (String)rpressure);
}

void print_LCD(int fpressure, int rpressure){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("FPres: " + (String)fpressure);
    lcd.print("psi");
    lcd.setCursor(0,1);
    lcd.print("RPres: " + (String)rpressure);
    lcd.print("psi");
}

void print_File(int fpressure, int rpressure) {
    myFile.print(rtc.getTimeStr());
    myFile.print(" ");
    myFile.print("FPres: " + (String)fpressure);
    myFile.print(" ");
    myFile.print("RPres: " + (String)rpressure);
    myFile.print("\n");
}

void loop() {
  fpressure, rpressure = analogRead(analog1), analogRead(analog2);
  recording = digitalRead(digital0);

  print_Serial(fpressure, rpressure);
  print_LCD(fpressure, rpressure);

  if (recording == HIGH) {
      print_File(fpressure, rpressure);
  }
  if (recording != last_recording) {
      if (recording == LOW){
          myFile = SD.open("test.txt",FILE_WRITE);
          Serial.println("New file made!");
          recording = HIGH;
      } else {
          myFile.close();
          recording = LOW;
          Serial.println("Closing...");
      }
  }
  last_recording = recording;
  delay(1000);
}

//const char* getFileName()
//{
//  String time_now = rtc.getTimeStr();
//  String date_now = rtc.getDateStr();
//  time_now.remove(2,1);
//  time_now.remove(4,1);
//  date_now.remove(2,8);
//  strcpy (filename,time_now.c_str());
//  strcat (filename,date_now.c_str());
//  strcat (filename,".txt");
//  Serial.print(filename);
//  return filename;
//}
