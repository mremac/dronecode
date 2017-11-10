#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include "Wire.h"
#endif

#include <Servo.h>  
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CNS, CE
const byte address[6] = "00001";

MPU6050 accelgyro;
int16_t ax, ay, az;
int axa, aya, aza;
int16_t gx, gy, gz;
#define OUTPUT_READABLE_ACCELGYRO
#define LED_PIN 13
bool blinkState = false;

Servo servo1;
Servo servo2;
Servo servo3;
Servo servo4;

int pos = 0;
int offa, offb, offc, offd;
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
   
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
    char text[32] = "";
    radio.read(&text, sizeof(text));
    //Check first for radio signal
    //!strcmp(text, "Hello World")
    if(2){
      //Print received signal and then start rotors - rotors will speed up a little
      Serial.println(text);
       for(pos=50;pos<80;pos += 3){
          servo1.write(pos);
          servo2.write(pos);
          servo3.write(pos);
          servo4.write(pos);
          delay(1000);
          radio.read(&text, sizeof(text));
          
          Serial.println(text);
          
          
       }
       
       // Reset all offsets and speeds ready to begin RC
       pos = 38;
       axa = 2900;
       aya = 240;
       aza = 23000;
       offa = 0;
       offb = 0;
       offc = 0;
       offd = 0;

       //slow down motors
      servo1.write(pos);
      servo2.write(pos);
      servo3.write(pos);
      servo4.write(pos);
    } else{
//      Serial.println(text);
    }
 }

 void loop(){
    // blink LED to indicate activity
    blinkState = !blinkState;
    digitalWrite(LED_PIN, blinkState);
    //radio.available()
  if (2) {
    
    char text[32] = "";
    radio.read(&text, sizeof(text));
      Serial.println(text);
      String str(text);
      int remoteSpeed = str.toInt();
      remoteSpeed = remoteSpeed/10;
//    radio.read(&text, sizeof(text));
    
    // read raw accel/gyro measurements from device
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

    // Use data to balance/move drone
    feedbackCheckAccelGyro();

    // Print data
    printAccelGyro();
          
//    if(strcmp(text, "Hello world" == 1)){
//      Serial.println(text);

      Serial.print(offa); Serial.print("\t");
      Serial.print(offb); Serial.print("\t");
      Serial.print(offc); Serial.print("\t");
      Serial.print(offd); Serial.print("\t");

//      Serial.println("this butt");

//      servo1.write(pos + offa);
//      servo2.write(pos + offb);
//      servo3.write(pos + offc);
//      servo4.write(pos + offd);

      servo1.write(remoteSpeed);
      servo2.write(remoteSpeed);
      servo3.write(remoteSpeed);
      servo4.write(remoteSpeed);
      delay(200);
      
//    } else{
//      Serial.println(text);
//    }
  } else{
    Serial.println("No radio");
      delay(500);
  }


//       for(pos=26;pos<80;pos += 3){
//         servo1.write(pos);
//         servo2.write(pos);
//         servo3.write(pos);
//         servo4.write(pos);
//         delay(1000);
//       }

    
     
 }

 void feedbackCheckAccelGyro(){
  if(az < aza - 500){
    offa ++;
    offb ++;
    offc ++;
    offd ++;
  } else if(ax > aza + 500){
    offa --;
    offb --;
    offc --;
    offd --;
  }

//  if(ay < aya - 500){
//    offa ++;
//    offb ++;
//    offc --;
//    offd --;
//  } else if(ay > aya + 500){
//    offa --;
//    offb --;
//    offc ++;
//    offd ++;
//  }
//
//  if(ax < axa - 500){
//    offa ++;
//    offb --;
//    offc ++;
//    offd --;
//  } else if(ax > axa + 500){
//    offa --;
//    offb ++;
//    offc --;
//    offd ++;
//  }
 }

 void printAccelGyro(){
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
