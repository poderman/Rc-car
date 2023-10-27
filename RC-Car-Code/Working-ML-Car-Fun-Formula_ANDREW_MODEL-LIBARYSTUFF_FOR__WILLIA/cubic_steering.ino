/*void myCUBIC_STEERING() {

  float P1 = (myX1 + (myWidth1 / 2.0)) - 48.0;

  //P1 = P1 + 1 ;

  //float P1 = ((((myX1 + (myWidth1 / 2)) + (myX2 + (myWidth2))) / 2) - 48);

  float STRG = (((1.0 / 900.0) * (pow(P1,3.0))) + 93.0);

  STEER = STRG;

  if (STRG <= 63.0) {
    STEER = 63.0;
  }

  if (STRG >= 123.0) {
    STEER = 123.0;
    //P1 = 63;
  }

  Steering_servo.write(STEER);
  Serial.println("Angle: " + String(STEER));

}

/*void LINEAR_STEERING() {

  float P1 = (myX1 + (myWidth1 / 2.0)) - 48;

  //float P1 = ((((myX1 + (myWidth1 / 2)) + (myX2 + (myWidth2))) / 2) - 48);

  float STRG = P1 + 93;

  STEER = STRG;

  if (STRG <= 63) {
    STEER = 63;
  }

  if (STRG >= 123) {
    STEER = 123;
  }

  Steering_servo.write(STEER);
  Serial.println("Angle: " + String(STEER));

}//*/
