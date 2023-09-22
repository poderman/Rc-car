/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file BRAKE.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.3.3
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _BRAKE_
#define _BRAKE_

void Brake()
{

#ifdef USE_BRAKE

#ifdef COLORS
  digitalWrite(LEDB, HIGH);
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, LOW);
#endif

  analogWrite(CONTROL_PIN, 0); // set Speed to Zero

  digitalWrite(D5, 1); // set one direction
  digitalWrite(D3, 1); // set one direction

#ifdef LINE_OUT_DRIVING

  Serial.println("Braking!!!");

#endif

#endif
}

#endif