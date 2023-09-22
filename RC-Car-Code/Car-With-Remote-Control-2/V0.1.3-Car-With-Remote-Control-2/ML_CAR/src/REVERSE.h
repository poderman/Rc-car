/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file REVERSE.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.1.0
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _REVERSE_
#define _REVERSE_

void Reverse(volatile int(Speed) = MIN_SPEED)
{

  digitalWrite(FORWARD_PIN, 0); // set one direction
  digitalWrite(REVERSE_PIN, 1); // set one direction

  if (Speed >= 64)
  { // make sure we dont exceeed our max value for speed
    Speed = 64;
  }

  analogWrite(CONTROL_PIN, Speed); // go Value of Speed

#ifdef LINE_OUT_DRIVING

  Serial.println("Speed: " + String(Speed));

#endif
}

#endif