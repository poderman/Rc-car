/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file COAST.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.2.1
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _COAST_
#define _COAST_

void Coast()
{

#ifdef USE_COAST

  analogWrite(CONTROL_PIN, 0); // set Speed to Zero

  digitalWrite(D5, 0); // set one direction
  digitalWrite(D3, 0); // set one direction

#endif

#ifdef LINE_OUT_DRIVING

  Serial.println("Coasting");

#endif
}

#endif