#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <virtuabotixRTC.h>     //Kütüphanemizi ekliyoruz.
#include <SPI.h>
#include <SD.h>

const int sicaklik_pin=A0;
const int chipSelect = 4;
int sicaklik = 0;
String anlikSicaklik = "0 C";

                                  
virtuabotixRTC myRTC(6,7,8);      // Kütüphanemizi pinlere atıyoruz.
LiquidCrystal_I2C lcd(0x27,16,2);  // 16 karakter 2 satır için - 20x4 satır için (0x27,20,4) yazın

void setup()
{ 
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
  } else {
    Serial.println("Card is active.");
  }
 lcd.begin (2,16); //Initialize the LCD,
 lcd.init();
 lcd.backlight();
 myRTC.setDS1302Time(00, 32, 16, 3, 19, 12, 2018);
 lcd.print("TEST");
}

void loop() {

  sicaklik = analogRead(sicaklik_pin); //Ham analog veriyi değişkenimizde sakladık.
  sicaklik = sicaklik*0.048828125; //Celcius cinsinden sıcaklık birimine dönüştürdük.
  anlikSicaklik = sicaklik+String(" C");
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile) {
    dataFile.println(anlikSicaklik+String(" [")+myRTC.hours+String(":")+myRTC.minutes+String(":")+myRTC.seconds+String("]"));
    Serial.println(anlikSicaklik+String(" [")+myRTC.hours+String(":")+myRTC.minutes+String(":")+myRTC.seconds+String("]"));
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  }
 lcd.clear(); //Here after clearing the LCD we take the time from the module and print it on the screen with usual LCD functions
 myRTC.updateTime();
 lcd.setCursor(0,0);
 lcd.print(myRTC.dayofmonth);
 lcd.print("/");
 lcd.print(myRTC.month);
 lcd.print("/");
 lcd.print(myRTC.year);
 lcd.setCursor(0,1);
 lcd.print(myRTC.hours);
 lcd.print(":");
 lcd.print(myRTC.minutes);
 lcd.print(":");
 lcd.print(myRTC.seconds);
 lcd.setCursor(10,1);
 lcd.print(anlikSicaklik);
 delay(1000);
}
