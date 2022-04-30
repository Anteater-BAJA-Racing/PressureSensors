#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <string.h>

// Real Time Clock
// RTC_PCF8523 RTC;

// Analog Pins
int analog0 = A0;
int analog1 = A1;


// Digital Pins (for recording)
int recording_toggle = 2;

// Input Variables
int front_input;
int rear_input;

char front_title[] = "FronBrake";
char rear_title[] = "RearBrake";

// Recording Variables
int recording;
int last_recording = LOW;

// File
File logfile;
char filename[] = "Tele00.txt";

// SD Card
const int chipSelect = 10;
int sd;

// Mode
bool serial = true;
bool storage = true;

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

void print_serial(char name[], int input) {
  Serial.println((String)name + ": " + (String)input);
}

void print_file(char name[], int input) {
  logfile.println((String)name + ": " + (String)input);
}

int record_data(char name[], int input) {
  if (storage == true) {
    print_file(name, input);
  }
  if (serial == true) {
    print_serial(name, input);    
  }
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
  front_input = analogRead(analog0);
  rear_input = analogRead(analog0);
  recording = digitalRead(recording_toggle);
}

void loop() {
  sd = check_sd();
  if(sd) {
    recording = digitalRead(recording_toggle);
    front_input = analogRead(analog0);
    rear_input = analogRead(analog1);

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
      record_data(front_title, front_input);
      //record_data(rear_title, rear_input);
    }
    else {
      // Do Nothing
      // Serial.println("Recording Pending");
    }
    delay(1000);
  }
  else {
    check_sd(false, true);
    delay(10000);
  }
}
