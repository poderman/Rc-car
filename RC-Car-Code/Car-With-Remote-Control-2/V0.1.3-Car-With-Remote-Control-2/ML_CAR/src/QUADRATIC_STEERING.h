/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file QUADRATIC_STEERING.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.22.5
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _QUADRATIC_STEERING_
#define _QUADRATIC_STEERING_

void QUADRATIC_STEERING()
{

#ifdef ONE_OBJECT

  volatile float P1 = (myX1 + (myWidth1 / 2)) - 48;

#endif

#ifdef TWO_OBJECT

  volatile float P1 = ((((myX1 + (myWidth1 / 2)) + (myX2 + (myWidth2))) / 2) - 48);

#endif

  volatile float P2 = ((-1.0 / 30.0) * (pow(P1, 2))) + MID_POINT;
  volatile float StL = (MID_POINT - (MID_POINT - P2));
  volatile float StR = (MID_POINT + (MID_POINT - P2));

  StL = constrain(StL, LOW_POINT, MID_POINT);

  StR = constrain(StR, MID_POINT, HIGH_POINT);

  /*if (StL <= LOW_POINT)
  {
    StL = LOW_POINT;
  }

  if (StR >= HIGH_POINT)
  {
    StR = HIGH_POINT;
  }//*/

  if (P1 > 0)
  {

#ifdef LINE_OUT_STEERING
    Serial.println(F("RIGHT"));
    Serial.println(String(StL));
#endif

    STEER = StR;
    Steering_servo.write(STEER);
  }

  if (P1 < 0)
  {

#ifdef LINE_OUT_STEERING
    Serial.println(F("LEFT"));
    Serial.println(String(StR));
#endif

    STEER = StL;
    Steering_servo.write(STEER);
  }

#ifdef LINE_OUT_STEERING
  Serial.println("Angle: " + String(STEER));
#endif
}

#endif