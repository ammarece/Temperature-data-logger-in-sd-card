/*
Code Name: Temperature and Humidity data logger into SD card 
Author: ammarece
Description: This program used for storing temperature and humidity data of sensor
License: Remixing or Changing this Thing is allowed. Commercial use is not allowed.
*/
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SD.h>

#define DHTPIN 2          // DHT11 data pin
#define DHTTYPE DHT11     // DHT11 sensor type

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x3F, 16, 2); // set the LCD address to 0x3F for a 16 chars and 2 line display

const int chipSelect = 4;         // SD card chip select pin
File dataFile;                    // File object to write data to SD card

void setup() {
  Serial.begin(9600);
  lcd.init();                      // initialize the LCD
  lcd.backlight();                 // turn on the backlight
  dht.begin();                     // start the DHT11 sensor

  // initialize SD card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");
}

void loop() {
  float h = dht.readHumidity();    // read humidity value
  float t = dht.readTemperature(); // read temperature value
  if (isnan(h) || isnan(t)) {      // check for NaN values
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(t);
  lcd.print("C");

  lcd.setCursor(0, 1);
  lcd.print("Humidity: ");
  lcd.print(h);
  lcd.print("%");

  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print("C   Humidity: ");
  Serial.print(h);
  Serial.println("%");

  // write temperature and humidity data to SD card
  dataFile = SD.open("datalog.txt", FILE_WRITE);
  if (dataFile) {
    dataFile.print(t);
    dataFile.print(",");
    dataFile.print(h);
    dataFile.print(",");
    dataFile.println();
    dataFile.close();
    Serial.println("Data written to SD card.");
  } else {
    Serial.println("Error opening datalog.txt file.");
  }

  delay(2000);                     // wait 2 seconds before reading again
}
