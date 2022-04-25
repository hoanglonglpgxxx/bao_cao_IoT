#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include<SoftwareSerial.h>


SoftwareSerial Arduino_SoftSerial (10,11); // RX,TX
LiquidCrystal_I2C lcd(0x27, 20,4);

#include "DHT.h"            
const int DHTPIN = A0;      
const int DHTTYPE = DHT11;  
int threshold = 300; // ngưỡng của cảm biến khí gas
int flame;
int flame_sen = 9; 
int thermo = A0; 
 int gas_sen =A1;
int buzzer = 8;

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Arduino_SoftSerial.begin(9600);
  lcd.init();
  lcd.backlight();
  lcd.setCursor(6,0); 
  lcd.print("Bao cao IoT");
  lcd.setCursor(3,1); 
  lcd.print("Thiet ke mach");
  lcd.setCursor(0,2); 
  lcd.print("bao chay & chua chay ");
  delay(2000);
}

void loop() {
  flame = digitalRead(flame_sen);
  int gas_val = analogRead(gas_sen);
  int tempc = dht.readTemperature();     
  
  
  lcd.clear();
  lcd.setCursor(1,0);
  lcd.print("ht bao & chua chay");
  lcd.setCursor(0,1);
  lcd.print("temp: ");
  lcd.print(tempc);
  Serial.print("temp: ");
  Serial.println(tempc);
  lcd.setCursor(11,1);
  lcd.print("Gas: ");
  lcd.print(gas_val);
  Serial.print("Gas: ");
  Serial.println(gas_val);
  

//  if(flame == 0 && analogRead(gas_sen) > threshold) {
//    lcd.setCursor(0,3);
//    lcd.print("phat hien lua va gas");
//    Serial.println("phat hien lua va gas");
//    both_alarm();
//
//  }
  
//  if(flame==0 && analogRead(gas_sen) > threshold) {
//     lcd.print("Phat hien  lua");
//    lcd.setCursor(0,3);
//    lcd.print("Khong phat hien gas");
////    Serial.println("Phat hien  lua");
//    flame_alarm();
//  }
  if(flame == 1)  {
    lcd.setCursor(3,2);
    lcd.print("Phat hien  lua");
    lcd.setCursor(0,3);
    lcd.print("Khong phat hien gas");
//    Serial.println("Phat hien  lua");
    flame_alarm();
  }
  else if(analogRead(gas_sen) > threshold) {
    lcd.setCursor(0,2);
    lcd.print("Khong phat hien lua");
    lcd.setCursor(2,3);
    lcd.print("Phat hien gas");
    gas_alarm();
//    Serial.println("Phat hien gas");
  }
    else{
      lcd.setCursor(0,3);
      lcd.print("Khong co gas va lua");
      Serial.println("Khong co gas va lua");
    }
    Arduino_SoftSerial.print(tempc);          Arduino_SoftSerial.print("A");
    Arduino_SoftSerial.print(gas_val);        Arduino_SoftSerial.print("B");
    Arduino_SoftSerial.print(flame);          Arduino_SoftSerial.print("C"); 
    Arduino_SoftSerial.print("\n");
    delay(2000);
  }

void gas_alarm() {
  tone(buzzer,2000,700);
}

void flame_alarm() {
  tone(buzzer,1500,700);
}
