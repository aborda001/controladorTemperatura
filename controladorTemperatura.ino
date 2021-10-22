#include <Thermistor3.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

RTC_DS1307 rtc;
Thermistor temp(0); //begin object Thermistor in pin 0
double resistor = 10000; //value of resistor in ohms
int sample = 5000; //number of samples
int pins[5] = {2, 3, 4, 5, 6}; //pins to use
int pinsSize = 5; //pins array size
int tempMax = 25; //max temp to turn on the pins
int firstCycle[2] = {6,8}; //first cycle to turn on the pins
int secondCycle[2] = {16,18}; //second cycle to turn on the pins
long int interval = 3000; //time interval between switching on the pins
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  rtc.begin();
  
  if (rtc.isrunning()) {
    //sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("RTC is not running");
  }

  for (int pin = 0; pin < pinsSize; pin++) {
    //initialize pins to 'LOW' and 'OUTPUT' mode
    pinMode(pins[pin], OUTPUT);
    digitalWrite(pins[pin], LOW);
  }

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Controlador de");
  lcd.setCursor(0, 1);
  lcd.print("temperatura.");
  delay(3000);
  lcd.clear();
}

void loop() {
  DateTime time = rtc.now();
  String hora = time.timestamp(DateTime::TIMESTAMP_TIME);
  char buf1[] = "mm";
  char buf2[] = "hh";
  String minutes = time.toString(buf1);
  String hours = time.toString(buf2);
  int mm = minutes.toInt();
  int hh = hours.toInt();

  double averageTemp = Temperatura();
  putTime(mm, hh);

  if ((hh >= firstCycle[0]) && (hh < firstCycle[1])) {
    if (averageTemp > tempMax) {
      for (int i = 0; i < pinsSize; i++) {
        ON(pins[i], interval);
        OFF(pins[i]);
        putTime(mm, hh);
      }
    }
  }
  if ((hh >= secondCycle[0]) && (hh < secondCycle[1])) {
    if (averageTemp > tempMax) {
      for (int i = 0; i < pinsSize; i++) {
        ON(pins[i], interval);
        OFF(pins[i]);
        putTime(mm, hh);
      }
    }
  }
}


double Temperatura() {
  double average = 0;
  //Read the data and plus to variable average
  for (int index = 0; index < sample; index++) {
    double temperature = temp.getTemp(resistor);
    average += temperature;
    // a small delay for read again
    delayMicroseconds(1);
  }
  // get the average
  average = average / sample;
  //print on LCD the average temperature
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(average);
  lcd.print(" C");
  
  return average;
}

void ON(int pin, int interval) {
  //Turn on the pin and print on LCD
  digitalWrite(pin, HIGH);
  lcd.setCursor(0, 4);
  lcd.print("Pin ");
  lcd.print(pin);
  lcd.print(" Encendido");
  delay(interval);
}

void OFF(int pin) {
  //Turn off the pin and clear second row on LCD
  digitalWrite(pin, LOW);
  lcd.setCursor(0, 1);
  lcd.print("                    ");
}

void putTime(int mm, int hh) {
  //Put time on LCD 
  lcd.setCursor(11, 0);
  lcd.print(hh);
  lcd.print(":");
  lcd.print(mm);
}
