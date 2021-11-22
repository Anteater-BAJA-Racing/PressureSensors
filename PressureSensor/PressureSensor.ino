#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <SPI.h>
#include <SD.h>
#include <DS3231.h>
#include <string.h>

int analog0;
int analog1;
int analog2;
String fpressure;
String rpressure;
float cal;
bool recording = false;
char filename[12];
File myFile;
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DS3231  rtc(SDA, SCL);

void setup()
{
  Serial.begin(9600);
  rtc.begin();
  lcd.begin();
  lcd.clear();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("FPres:");
  lcd.setCursor(0,1);
  lcd.print("RPres:");
  myFile.close();
  //uncomment this to recalibrate the clock
  //rtc.setDOW(THURSDAY);
  //rtc.setTime(1,21,00);
  //rtc.setDate(11, 11, 2021);
}


void loop()
{
  analog0 = analogRead(A0);
  analog1 = analogRead(A1);
  analog2 = analogRead(A2);
  Serial.println(analog0);
  Serial.println(analog1);
  lcd.clear();
  lcd.setCursor(0,0);
  String fpressure = "FPres: " + (String)analog1;
  String rpressure = "RPres: " + (String)analog2;
  lcd.print(fpressure);
  lcd.print("psi");
  lcd.setCursor(0,1);
  lcd.print(rpressure);
  lcd.print("psi");
  if(analog0 > 100 && recording == LOW)
  {
    myFile = SD.open("test.txt",FILE_WRITE);
    Serial.println("New file made!");
    recording = HIGH;
  }
  if(recording == HIGH)
  {
      myFile.print(rtc.getTimeStr());
      myFile.print(" ");
      myFile.print(fpressure);
      myFile.print(" ");
      myFile.print(rpressure);
      myFile.print("\n");
      Serial.println("Printing...");
  }
  if(analog0 < 100 && recording == HIGH)
  {
      myFile.close();
      recording = LOW;
      Serial.println("Closing...");
  }
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
