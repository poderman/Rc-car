/**
 * 01000100 01000001 01010010 01010100 01001000 00100000 01010110 01000001 01000100 01000101 01010010
 *
 * @file CUBIC_STEERING.h
 * @author BRANDON KENNARD (SLASHSPYRO@GMAIL.COM)
 * @brief
 * @version 0.15.2
 * @date 15-11-22
 *
 * @copyright NONE 2022
 *
 **/

#ifndef _CUBIC_STEERING_
#define _CUBIC_STEERING_

/*

if (((HIGH_POINT - MID_POINT) - (MID_POINT - LOW_POINT)) != 0)
{

  int Q = (HIGH_POINT - ((HIGH_POINT - MID_POINT) - (MID_POINT - LOW_POINT)));
}

Y=A(B(X^3))+D

A=CBRT(Y-D)/BX

B=CBRT((Y-D)/A)/X

X=((CBRT(Y-D))A)/B

D=Y-A(BX)^3

L = LEFT

M = MID = D

R = RIGHT

B=RT(((L-M)X^2)/(CBRT(L-D)))

A=ABS((CBRT(L-M))/(BX))

B=cbrt((Y-D)/(abs((cbrt(L-M))/((sqrt(((L-M)X^2)/(cbrt(L-D))))X))))/X

A=ABS((CBRT(L-M))/((CBRT((Y-D)/(ABS((CBRT(L-M))/((RT(((L-M)X^2)/(CBRT(L-D))))X))))/X)X))

int L = (LOW_POINT + Q);

int five = pow(((LOW_POINT - MID_POINT) * VIEW_AREA), 2);
int seven = pow((LOW_POINT - MID_POINT), (1 / 3));

int B = sqrt(five) / sqrt(seven); //*/

void CUBIC_STEERING()
{

#ifdef ONE_OBJECT

  volatile float P1 = (myX1 + (myWidth1 / 2.0)) - 48;

#endif

#ifdef TWO_OBJECT

  volatile float P1 = ((((myX1 + (myWidth1 / 2)) + (myX2 + (myWidth2))) / 2) - 48);

#endif

#ifndef VIEW_AREA

  // #define VIEW_AREA 30

  volatile float STRG = (((1.0 / 900.0) * (pow(P1, 3.0))) + MID_POINT);

#endif

  volatile float STRG = (((1.0 / 900.0) * (pow(P1, 3.0))) + MID_POINT);

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