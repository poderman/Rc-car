/*float STEER = 93;

//float P1 = ((((myX1 + (myWidth1 / 2)) + (myX2 + (myWidth2))) / 2) - 48);
//
//float P2 = ((-1.0 / 30.0) * (pow(P1, 2))) + 0.0 * P1 + 93.0;
//float StL = (93 - (93 - P2));
//float StR = (93 + (93 - P2));

void Steering(char Control) {

  if (Control == 'L') {
    Steering_Left();
    //Serial.println("Left");
  }
  if (Control == 'R') {
    Steering_Right();
    //Serial.println("Right");
  }
  if (Control == 'S') {
    Steering_Straight();
    //Serial.println("Straight");
  }

}

void Single_Pro_SteeringR() {

  float P1 = (myX1 + myWidth1);
  float P2 = ((-1.0 / 30.0) * (pow(P1, 2))) + 93.0;
  float StR = (93 + (93 - P2));


  if (StR >= 123) {
    StR = 123;
  }


  if ((P1 <= 0) == false) {

    STEER = StR;
  } else {
    STEER = 93;
  }

  Steering_servo.write(STEER);

}

void Single_Pro_SteeringL() {

  float P1 = (myX1 + myWidth1);
  float P2 = ((-1.0 / 30.0) * (pow(P1, 2))) + 93.0;
  float StL = (93 - (93 - P2));

  if (StL <= 63) {
    StL = 63;
  }

  if ((P1 >= 96) == false) {

    STEER = StL;
  } else {
    STEER = 93;
  }

  Steering_servo.write(STEER);

}

void Pro_Steering() {

  //float STEER

  float P1 = (myX1 + (myWidth1 / 2)) - 48;

  //float P1 = ((((myX1 + (myWidth1 / 2)) + (myX2 + (myWidth2))) / 2) - 48);

  float P2 = ((-1.0 / 30.0) * (pow(P1, 2))) + 93.0;
  float StL = (93 - (93 - P2));
  float StR = (93 + (93 - P2));

  if (StL <= 63) {
    StL = 63;
  }

  if (StR >= 123) {
    StR = 123;
  }

  if (P1 > 0) {
    //Serial.println(F("RIGHT"));
    //Serial.println(String(StL));
    STEER = StR;
    Steering_servo.write(STEER);
  }

  if (P1 < 0) {
    //Serial.println(F("LEFT"));
    //Serial.println(String(StR));
    STEER = StL;
    Steering_servo.write(STEER);
  }

  Serial.println("Angle: " + String(STEER));

}


void Steering_Left() {

  Steering_servo.write(63); //58 max left

}

void Steering_Right() {

  Steering_servo.write(123); //132 max right

}


void Steering_Straight() {

#ifdef Use_Straight

  Steering_servo.write(93); //93 equal straight
  //delay(200);
#endif
}//*/
