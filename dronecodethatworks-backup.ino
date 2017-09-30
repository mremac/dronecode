 #include <Servo.h> 
 
 #include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
RF24 radio(9, 10); // CNS, CE
const byte address[6] = "00001";

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int pos = 0;
int i = 0;

 #define rfReceivePin A0  //RF Receiver pin = Analog pin 0
 #define ledPin 13        //Onboard LED = digital pin 13

 unsigned int data = 0;   // variable used to store received data
 const unsigned int upperThreshold = 0;  //upper threshold value
 const unsigned int lowerThreshold = 0;  //lower threshold value

 void setup(){
   Serial.begin(9600);
 servo1.attach(4);
 servo2.attach(5);
 servo3.attach(6);
 servo4.attach(7);
     pos = 26;
   pinMode(ledPin, OUTPUT);
   Serial.begin(9600);
   
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
 }

 void loop(){
//   data=analogRead(rfReceivePin);    //listen for data on Analog pin 0
//   
//   if(data>upperThreshold){
//     digitalWrite(ledPin, LOW);   //If a LOW signal is received, turn LED OFF
//     Serial.println(data);
//     Serial.println(2);
//   }
//   
//   if(data<lowerThreshold){

    
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
      Serial.println(text);
    if(!strcmp(text, "Hello World")){
       for(pos=35;pos<95;pos += 3){
         if(!strcmp(text, "Hello World")){
         servo1.write(pos);
         servo2.write(pos);
         servo3.write(pos);
         servo4.write(pos);
         delay(1000);
         radio.read(&text, sizeof(text));
         Serial.println(text);
         } else{
//          Serial.println(text);
          pos = 26;
          servo1.write(pos);
          servo2.write(pos);
          servo3.write(pos);
          servo4.write(pos);
         }
       }
    }
  }else{
    Serial.println("wtf");
    
         servo1.write(0);
         servo2.write(0);
         servo3.write(0);
         servo4.write(0);
  }


//       for(pos=26;pos<80;pos += 3){
//         servo1.write(pos);
//         servo2.write(pos);
//         servo3.write(pos);
//         servo4.write(pos);
//         delay(1000);
//       }

    
     
 }
