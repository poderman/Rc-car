/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file DRIVING.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.5.0
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _DRIVING_
#define _DRIVING_

void INIT_MOTOR()
{
    // Big Motor Contol Pins
    pinMode(REVERSE_PIN, OUTPUT); // digital 0 to 1
    pinMode(CONTROL_PIN, OUTPUT); // PWM 0 to 255
    pinMode(FORWARD_PIN, OUTPUT); // digital 0 to 1
}

#include <FORWARD.h>
#include <REVERSE.h>
#include <BRAKE.h>
#include <COAST.h>

#endif