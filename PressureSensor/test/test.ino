#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

// Real Time Clock
// RTC_PCF8523 RTC;

// Analog Pins
int analog0 = A0;

// Digital Pins (for recording)
int recording_toggle = 2;

// Input Variables
int input;

// Recording Variables
int recording;
int last_recording = LOW;

// File
File logfile;
char filename[] = "Tele00.txt";

// SD Card
const int chipSelect = 10;
int sd;

// Functions
int check_sd(bool first_run=false, bool print_info=false) {
  if(first_run) {
    Serial.print("Initializing SD card... ");
  }
  else if(print_info) {
    Serial.print("Checking SD card... ");
  }

  if(!SD.begin(chipSelect)) {
    if(print_info) {
      Serial.println("card failed, or not present");
    }
    return 0;
  }
  else {
    if(print_info) {
      Serial.println("card initialized.");
    }
    return 1;
  }
}

void open_file() {
  for (uint8_t i = 0; i < 100; i++) {
    filename[4] = i/10 + '0';
    filename[5] = i%10 + '0';
    if (! SD.exists(filename)) {
      logfile = SD.open(filename, FILE_WRITE); 
      break;
    }
  }
}

void close_file() {
  logfile.close();
}

void print_serial(int input) {
  Serial.println("Input: " + (String)input);
}

void print_file(int input) {
<<<<<<< HEAD
  logfile.println("Input: " + (String)input);
}

int record_data(int input) {
  print_file(input);
  print_serial(input);
=======
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
>>>>>>> c71c75924164b57f3d62a90429757f2b535354cf
}

void setup() {
  Serial.begin(9600);

  sd = check_sd(true, true);
  while(!sd) {
    sd = check_sd(false, true);
    delay(10000);
  }

  // Setting Pins
  pinMode(analog0, INPUT);
  pinMode(recording_toggle, INPUT_PULLUP);

  // Input Variables
  input = analogRead(analog0);
  recording = digitalRead(recording_toggle);
}

void loop() {
  sd = check_sd();
  if(sd) {
    recording = digitalRead(recording_toggle);
    input = analogRead(analog0);
    
    if (last_recording != recording) {
      if (recording == HIGH) {
        last_recording = recording;
        open_file();
        Serial.println("Recording Starting");
      }
      else {
        last_recording = recording;
        close_file();
        Serial.println("Recording Ending");
      }
    }
    else if (recording == HIGH) {
      record_data(input);
    }
    else {
      // Do Nothing
      // Serial.println("Recording Pending");
    }
    delay(1000);
  }
<<<<<<< HEAD
  else {
    check_sd(false, true);
    delay(10000);    
=======
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
>>>>>>> c71c75924164b57f3d62a90429757f2b535354cf
  }
}
