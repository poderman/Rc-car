/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file STEERING.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.10.2
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _STEERING_
#define _STEERING_

#include <Servo.h>

Servo Steering_servo;

void INIT_SERVO()
{
    Steering_servo.attach(SERVO_PIN, 500, 2500);
}

void INTI_STEERING()
{

    int V;

    switch ((MID_POINT - LOW_POINT) < (HIGH_POINT - MID_POINT))
    {
    case 1:
        V = LOW_POINT;

    case 0:
        V = HIGH_POINT;
    }

    // GENERATING VALUES FOR A CUBIC Y=A(B(X^3))+D
    long B = pow(VIEW_AREA, 2);
    B = B * (V - MID_POINT);
    B = pow(B, (1 / 2));
    long Z = V - MID_POINT;
    Z = pow(Z, (1 / 3));
    B = B / Z;

    long A = Z / (B * VIEW_AREA);
    A = abs(A);
    
}

volatile float STEER = MID_POINT;

volatile int myX1 = 48;
volatile int myX2 = 48;

volatile int myWidth1 = 10;
volatile int myWidth2 = 10;

#include <LINEAR_STEERING.h>
#include <QUADRATIC_STEERING.h>
#include <CUBIC_STEERING.h>

void Steering_Left()
{

    Steering_servo.write(LOW_POINT); // 58 max left
    //CurrentAngel = LOW_POINT;
}

void Steering_Right()
{

    Steering_servo.write(HIGH_POINT); // 132 max right
    //CurrentAngel = HIGH_POINT;
}

void Steering_Straight()
{

#ifdef USE_STRAIGHT

    Steering_servo.write(MID_POINT); // 93 equal straight
    //CurrentAngel = MID_POINT;

#endif
}

#endif