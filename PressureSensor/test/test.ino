#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

// Real Time Clock
// RTC_PCF8523 RTC;

// Analog Pins
int analog0 = A0;
int input;

// Digital Pins (for recording)
int recording = LOW;
int last_recording = HIGH;

// File
File dataFile;

// SD Card
const int chipSelect = 10;

void setup() {
  Serial.begin(9600);

  Serial.print("Initializing SD card...");

  if(!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    while (1);
  }
  
  Serial.println("card initialized.");
}

void print_serial(int input) {
  Serial.println("Input: " + (String)input);
}

void print_file(int input) {
  dataFile.println("Input: " + (String)input);
}

int record_data(int input) {
  if(dataFile) {
    //print_file(input);
    print_serial(input);
    return 1;
  }
  else {
    return 0;
  }
}

void loop() {
  input = analogRead(analog0);

  if (recording == HIGH) {
      record_data(input);
  }
  if (recording != last_recording) {
      if (recording == LOW){
        // FIX: file name should change
        dataFile = SD.open("test.txt", FILE_WRITE);
        dataFile.println("File Started");
        dataFile.close();
        recording = HIGH;
        record_data(input);
      } else {
        dataFile.close();
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
