#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include "Wire.h"
#endif

#include <Servo.h>  
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

MPU6050 accelgyro;
int16_t ax, ay, az;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO
#define LED_PIN 13
bool blinkState = false;

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
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif

  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  Serial.begin(9600);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU6050 connection successful" : "MPU6050 connection failed");
  pinMode(LED_PIN, OUTPUT);
  servo1.attach(4);
  servo2.attach(5);
  servo3.attach(6);
  servo4.attach(7);
  pos = 26;
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
   
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
 }

 void loop(){
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    
  if (radio.available()) {
    char text[32] = "";
    radio.read(&text, sizeof(text));
    if(2){
      Serial.println(text);
       for(pos=26;pos<80;pos += 3){
          servo1.write(pos);
          servo2.write(pos);
          servo3.write(pos);
          servo4.write(pos);
          delay(1000);
          radio.read(&text, sizeof(text));
          
          Serial.println(text);
          
          // read raw accel/gyro measurements from device
          accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
          #ifdef OUTPUT_READABLE_ACCELGYRO
              // display tab-separated accel/gyro x/y/z values
              Serial.print("a/g:\t");
              Serial.print(ax); Serial.print("\t");
              Serial.print(ay); Serial.print("\t");
              Serial.print(az); Serial.print("\t");
              Serial.print(gx); Serial.print("\t");
              Serial.print(gy); Serial.print("\t");
              Serial.println(gz);
          #endif
      
          #ifdef OUTPUT_BINARY_ACCELGYRO
              Serial.write((uint8_t)(ax >> 8)); Serial.write((uint8_t)(ax & 0xFF));
              Serial.write((uint8_t)(ay >> 8)); Serial.write((uint8_t)(ay & 0xFF));
              Serial.write((uint8_t)(az >> 8)); Serial.write((uint8_t)(az & 0xFF));
              Serial.write((uint8_t)(gx >> 8)); Serial.write((uint8_t)(gx & 0xFF));
              Serial.write((uint8_t)(gy >> 8)); Serial.write((uint8_t)(gy & 0xFF));
              Serial.write((uint8_t)(gz >> 8)); Serial.write((uint8_t)(gz & 0xFF));
          #endif
       }
    } else{
//      Serial.println(text);
    }
  }


//       for(pos=26;pos<80;pos += 3){
//         servo1.write(pos);
//         servo2.write(pos);
//         servo3.write(pos);
//         servo4.write(pos);
//         delay(1000);
//       }

    
     
 }