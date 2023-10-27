/*void Pin_Set() {

  /* FOR the GRAYSCALE Waveshare OLED 128 x 128 using library Adafruit_SSD1327.h
      yellow (sck) D9
      blue  DIN (mosi) D8
      orange (cs) D7
      green (dc)  D6
      white (reset) not needed but D14 if you did

  *//*

  //On Board LED
  pinMode(LEDR, OUTPUT);  //setup Built in Led Red
  pinMode(LEDG, OUTPUT);  //setup Built in Led Green
  pinMode(LEDB, OUTPUT);  //setup Built in Led Blue

  //Steering Servo Control Pins
  //Steering_servo.attach(SERVO_PIN, 500, 2500);

  //Big Motor Contol Pins
  pinMode(D3, OUTPUT);   // digital 0 to 1
  pinMode(D4, OUTPUT);   // PWM 0 to 255
  pinMode(D5, OUTPUT);   // digital 0 to 1

  digitalWrite(LEDR, HIGH);  //setup Built in Led Red
  digitalWrite(LEDG, HIGH);  //setup Built in Led Green
  digitalWrite(LEDB, HIGH);  //setup Built in Led Blue

}//*/
