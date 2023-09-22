/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file FORWARD.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.18.1
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _FORWARD_
#define _FORWARD_

void Foward(volatile int(Speed) = MIN_SPEED)
{

#ifdef COLORS
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDB, LOW);
  digitalWrite(LEDG, LOW);
#endif

  digitalWrite(FORWARD_PIN, 1); // set one direction
  digitalWrite(REVERSE_PIN, 0); // set one direction

  if (Speed < SLOW_SPEED)
  {
    Speed = SLOW_SPEED;
  }

  Speed = constrain(Speed, MIN_SPEED, FAST_SPEED);

  if (Speed >= 255)
  { // make sure we dont exceeed our max value for speed
    Speed = 255;
  }

  analogWrite(CONTROL_PIN, Speed); // go Value of Speed

#ifdef LINE_OUT_DRIVING

  Serial.println("Speed: " + String(Speed));

#endif
}

#endif