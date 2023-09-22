/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file LINEAR_STEERING.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.5.0
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _LINEAR_STEERING_
#define _LINEAR_STEERING_

void LINEAR_STEERING()
{

#ifdef ONE_OBJECT

  volatile float P1 = (myX1 + (myWidth1 / 2.0)) - 48;

#endif

#ifdef TWO_OBJECT

  volatile float P1 = ((((myX1 + (myWidth1 / 2)) + (myX2 + (myWidth2))) / 2) - 48);

#endif

#ifdef COLORS

  if (P1 < 0)
  {
    digitalWrite(LEDR, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDB, LOW);
  }

  if (P1 > 0)
  {
    digitalWrite(LEDB, HIGH);
    digitalWrite(LEDG, LOW);
    digitalWrite(LEDR, LOW);
  }

  if (P1 == 0)
  {
    digitalWrite(LEDG, HIGH);
    digitalWrite(LEDB, LOW);
    digitalWrite(LEDR, LOW);
  }

#endif

  volatile float STRG = P1 + MID_POINT;

  STEER = constrain(STRG, LOW_POINT, HIGH_POINT);

  /*STEER = STRG;

  if (STRG <= LOW_POINT)
  {
    STEER = LOW_POINT;
  }

  if (STRG >= HIGH_POINT)
  {
    STEER = HIGH_POINT;
  }//*/

  Steering_servo.write(STEER);

#ifdef LINE_OUT_STEERING

  Serial.println("Angle: " + String(STEER));

#endif
}

#endif