// RC Car servo and big motor driver
// By Jeremy Ellis
// MIT license

// for now reference at https://github.com/hpssjellis/particle.io-photon-high-school-robotics/tree/master/a11-dc-motor-drivers
// although pin names wrong

// You are suppossed to get it working using the web-app
// Draw your circuit diagram first
// This program will just tell you if  the connections are working
// See https://www.pololu.com/product/1451 for assistance

// On motor driver board LED goes red for one direction and green for the other






#include <Arduino.h> // Only needed by https://platformio.org/
#include <Servo.h>

Servo myServo_D2;

//counter = 0

void setup() {

  
    myServo_D2.attach(D2); // D2 should do PWM on Portenta
    pinMode(D3, OUTPUT);   // digital 0 to 1
    pinMode(D5, OUTPUT);   // PWM 0 to 255
    pinMode(D6, OUTPUT);   // digital 0 to 1
    
                            // both off = glide, both on = brake (if motor can do that) 
    digitalWrite(D5, 1);    // set one direction
    digitalWrite(D3, 0);    // set one direction 
    
    pinMode(LED_BUILTIN, OUTPUT);


//loop();{
//if counter ==5;{

 // Serial.print("hello world");
//}
//}
}

void loop() {

  //counter = counter + 5
    
    digitalWrite(LED_BUILTIN, LOW);    // D7 on

    myServo_D2.write(115); // turn
    analogWrite(D4, 50);   // go medium
    delay(1000);
    analogWrite(D4, 0);     // stop
    delay(3000); 
    
    myServo_D2.write(90);   // turn
    analogWrite(D4, 50);   // go medium   
    delay(1000);    
    analogWrite(D4, 0);     // stop
    delay(3000); 


    myServo_D2.write(65);  // go straight
    analogWrite(D4, 50);   // go medium   
    delay(1000);    
    analogWrite(D4, 0);     // stop
    delay(3000); 



    
    digitalWrite(LED_BUILTIN, HIGH);  // D7 off
    delay(9000);            // wait 9 seconds
        
}
