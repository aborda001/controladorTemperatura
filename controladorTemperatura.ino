#include <Thermistor3.h>
#include <LiquidCrystal_I2C.h>
#include "RTClib.h"

RTC_DS1307 rtc;
Thermistor temp(0);
double resistor = 10000;
int sample = 5000;
int pins[5] = {2,3,4,5,6};

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {

  for (int pin = 0; pin < 5; pin++) {
    pinMode(pins[pin], OUTPUT);
    digitalWrite(pins[pin], LOW);
  }
    
  Serial.begin(9600);
  rtc.begin();

  if (rtc.isrunning()) {
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  } else {
    Serial.println("RTC is not running");
  }

  lcd.init();
  lcd.clear();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Controlador de");
  lcd.setCursor(0, 1);
  lcd.print("temperatura.");
  delay(1000);
  lcd.clear();
}

void loop() {
  DateTime time = rtc.now();
  String hora = time.timestamp(DateTime::TIMESTAMP_TIME);
  char buf1[] = "mm";
  char buf2[] = "hh";
  String minutos = time.toString(buf1);
  String horario = time.toString(buf2);
  int mm = minutos.toInt();
  int hh = horario.toInt();

  Serial.println("Hora: ");
  Serial.println(hh);
  Serial.println("Minutos:  ");
  Serial.println(mm);

  
  Temperatura();
}


void Temperatura() {
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
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(average);
  lcd.print(" C");
  
  Serial.print("T: ");
  Serial.print(average);
  Serial.println(" C");
  delay(10);
  //lcd.clear();
}

void ON(int pin) {
  digitalWrite(pin,HIGH);
}
void OF(int pin) {
  digitalWrite(pin,LOW);
}
