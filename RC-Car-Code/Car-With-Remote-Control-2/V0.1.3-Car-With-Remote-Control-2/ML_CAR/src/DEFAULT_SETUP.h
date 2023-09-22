/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file DEFAULT_SETUP.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.1.0
 * @date 17-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _DEFAULT_SETUP_
#define _DEFAULT_SETUP_

/////////////////////////////EDGE IMPULSE DEFUALTS

#ifndef SLEEP

volatile int EI_SLEEP_DELAY = 1;

#endif

/////////////////////////////EDGE IMPULSE DEFUALTS END

/////////////////////////////STEERING DEFUALTS

#ifndef SERVO_PIN

#define SERVO_PIN -1

#endif

#ifndef ONE_OBJECT
#ifndef TWO_OBJECT

#define ONE_OBJECT

#endif
#endif

#ifndef MID_POINT

#define MID_POINT 90

#endif

#ifndef LOW_POINT

#define LOW_POINT 80

#endif

#ifndef HIGH_POINT

#define HIGH_POINT 100

#endif

/////////////////////////////STEERING DEFUALTS END

/////////////////////////////DRIVING DEFUALTS

#ifndef FORWARD_PIN

#define FORWARD_PIN -1

#endif

#ifndef REVERSE_PIN

#define REVERSE_PIN -1

#endif

#ifndef CONTROL_PIN

#define CONTROL_PIN -1

#endif

#ifndef MIN_SPEED

#define MIN_SPEED 50

#endif

/////////////////////////////DRIVING DEFUALTS END

#endif