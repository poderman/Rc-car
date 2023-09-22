/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file ML_CAR.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.15.0
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _ML_CAR_
#define _ML_CAR_

#include <edge-impulse-advanced-v2.h>

#include "mbed.h"
#include "rtos.h"

#include <DEFAULT_SETUP.h>

#ifdef SLEEP

volatile int EI_SLEEP_DELAY = SLEEP;

#endif

#ifdef MBED

using namespace mbed;

#endif

#ifdef RTOS

using namespace rtos;

#endif

#ifdef EXT_STEERING

#include <STEERING.h>

#endif

#ifdef EXT_DRIVING

#include <DRIVING.h>

#endif

void PIN_SETUP()
{
    // On Board LED
    pinMode(LEDR, OUTPUT); // setup Built in Led Red
    pinMode(LEDG, OUTPUT); // setup Built in Led Green
    pinMode(LEDB, OUTPUT); // setup Built in Led Blue

    digitalWrite(LEDR, HIGH); // setup Built in Led Red
    digitalWrite(LEDG, HIGH); // setup Built in Led Green
    digitalWrite(LEDB, HIGH); // setup Built in Led Blue

    delay(500);

    digitalWrite(LEDR, LOW); // setup Built in Led Red
    digitalWrite(LEDG, LOW); // setup Built in Led Green
    digitalWrite(LEDB, LOW); // setup Built in Led Blue

    delay(500);

    digitalWrite(LEDR, HIGH); // setup Built in Led Red
    digitalWrite(LEDG, HIGH); // setup Built in Led Green
    digitalWrite(LEDB, HIGH); // setup Built in Led Blue
}

#endif //_ML_CAR_