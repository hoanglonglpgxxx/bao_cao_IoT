#include<SoftwareSerial.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <Arduino.h>
#include <analogWrite.h>


SoftwareSerial NodeMcu_SoftSerial(27,14); // RX,TX

//define pump
#define enB  18 //4
#define in4  19 //16
#define in3  21 //17


//define fan
#define enA  26//25
#define in1  25//32
#define in2  33


BlynkTimer timer;

int buttonFan;
int buttonPump;
//led blynk
WidgetLED led_fan(V3);
WidgetLED led_pump(V4);

const char* ssid = "LongLe"; // Tên Wifi
const char* password = "Abc.2010@"; // Mật khẩu Wifi
char auth[] = "Z9O2nxFYxBw9RFNgD-Nqy0J7mBFdyk44";  // Mã Token từ blynk
//Below is Global Variable Data

int i;
int threshold = 300;
char c; 
String DataIn; 
int8_t indexofA, indexofB, indexofC;

String tempc, gas_val, flame;

void setup() {
  // put your setup code here, to run once:
  
  //Open Serial Communication (NodeMcu -PC)
  timer.run();
  Serial.begin(9600);


 pinMode(enB, OUTPUT);
 pinMode(in4, OUTPUT);
 pinMode(in3, OUTPUT);

 digitalWrite(in3, 0);
 digitalWrite(in4,0);
 analogWrite(enB, 0);
 
 pinMode(enA, OUTPUT);
 pinMode(in1, OUTPUT);
 pinMode(in2, OUTPUT);
 
 digitalWrite(in1, 0);
 digitalWrite(in2,0);
 analogWrite(enA, 0);
     

  //Open Serial communication (NodeMcu -NodeMcu)

  NodeMcu_SoftSerial.begin(9600);

  WiFi.begin(ssid, password);
  Blynk.begin(auth, ssid, password);
  led_fan.off();
  led_pump.off();

}

void loop() {
  Blynk.run();
  timer.run();
  // put your main code here, to run repeatedly:

  while(NodeMcu_SoftSerial.available()>0) {
    
    c= NodeMcu_SoftSerial.read();

    
    if(c== '\n') {break;} 
    else {DataIn+=c;}
  }

//   if(flame.toInt() == 1 && gas_val.toInt() > threshold) {
////     timer.setInterval(1000L, noti_both);
//     Serial.println("phat hien lua va gas");
//     led_fan.on();
//     led_pump.on();
//     fanTien();
//     pumpTien();
//     
//    }else
if(flame.toInt() == 1) {
//     timer.setInterval(1000L, noti_flame);
     noti_flame();
     Serial.println("Phat hien lua khong phat hien gas");
     led_pump.on();
     pumpTien();
     if(buttonFan == 1){
      led_fan.on();
      fanTien();
     }else{
      led_fan.off();
      fanDung();
     }
    }else if(gas_val.toInt() > threshold) {
//    timer.setInterval(1000L, noti_gas);
    noti_gas();
    Serial.println("Phat hien gas khong phat hien lua");
     led_fan.on();
     fanTien();
     if(buttonPump == 1){
      led_pump.on();
      pumpTien();
     }else{
      led_pump.off();
      pumpDung();
     }
     
    }
    else{
    Serial.println("Khong co gas va lua");
     if(buttonFan == 1){
      led_fan.on();
      fanTien();
     }else{
      led_fan.off();
      fanDung();
     }
      if(buttonPump == 1){
      led_pump.on();
      pumpTien();
     }else{
      led_pump.off();
      pumpDung();
     }
    }
    
    if(c== '\n')
    {
        Parse_the_data();


        //Show All data to the Serial Monitor

          Serial.println("tempc = " + tempc);
          Serial.println("gas_val = " + gas_val);
          Serial.println("flame = " + flame);
          Serial.println("=================================");

          Blynk.virtualWrite(V1,tempc);
          Blynk.virtualWrite(V2,gas_val);

        //Reset the variable

        c=0;
        DataIn="";  

    }

   
   }
    
   
//đọc giá trị button từ blynk
BLYNK_WRITE(V5) // this command is listening when something is written to V5, fan
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  buttonFan = pinValue;
  
}

BLYNK_WRITE(V6) // this command is listening when something is written to V6,pump
{
  int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable
  buttonPump = pinValue;

}


void fanDung() {
  digitalWrite(in1, 0);
  digitalWrite(in2, 0);
}
void fanTien() {
  digitalWrite(in1, 1);
  digitalWrite(in2, 0);
  analogWrite(enA, 200);
 
 }
 void pumpTien() {
  digitalWrite(in3, 1);
  digitalWrite(in4, 0);
  analogWrite(enB, 100);

}
void pumpDung() {
  digitalWrite(in3, 0);
  digitalWrite(in4, 0);
}

void noti_flame() {
Blynk.notify("Phat hien lua khong phat hien gas");
}
void noti_gas() {
Blynk.notify("Phat hien gas khong phat hien lua");
}
//void noti_both() {
//Blynk.notify("Phat hien lua va gas");
//}

void Parse_the_data()
{

  indexofA = DataIn.indexOf("A");
  indexofB = DataIn.indexOf("B");
  indexofC = DataIn.indexOf("C");


  tempc  = DataIn.substring (0, indexofA);
  gas_val  = DataIn.substring (indexofA+1, indexofB);
  flame  = DataIn.substring (indexofB+1, indexofC);

  
  }
