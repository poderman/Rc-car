int myCurrentObject;
int myPreviousObject;
int myPastObject;

char myCarDirection = 'N';

void Buffer() 
{
    myCurrentObject = myObjectCodeNew;//myObjectCode;//random(0, 3);

    // only if 2 myRands are the same
    //myCarDirection = myRand;
    /*Serial.print(myCurrentObject);
    Serial.print(", ");
    Serial.print(myPreviousObject);
    Serial.print(", ");
    Serial.print(myPastObject);
    Serial.print(", ");
    Serial.print(myCarDirection);
    Serial.println();
    delay(100);*/
    myPastObject = myPreviousObject;
    myPreviousObject = myCurrentObject;
}




void DrivingController_myDrivingThread() {

    while (true) 
    {

        /*float P1 = ((((myX1 + (myWidth1/2)) + (myX2 +(myWidth2)))/2)-48);*/

        //float P1Pow = pow(P1, 2);Serial.println("P1Pow" + String(P1Pow));
        //float P1Dec = (-1.000000/30.000000);Serial.println("DEC" + String(P1Dec));
        /*float P2 = ((-1.0/30.0)*(pow(P1,2)))+0.0*P1+93.0;
        float StL = (93-(93-P2));
        float StR = (93+(93-P2));*/




        //Serial.println("X1=" + String(myX1) + ", X2=" + String(myX2));
        //Serial.println("W1=" + String(myWidth1) + ", W2=" + String(myWidth2));

        //Serial.println("P1=" + String(P1));
        //Serial.println("P2=" + String(P2));


        //Serial.println(StL);
        //Serial.println(StR);

        delay(1);

        //EllisCarControl();
        //WorkingCarControl();
        //Buffer();
        //BufferControl();

        if (myObjectCode == 1) 
        {
            //CUBIC_STEERING();
            LINEAR_STEERING();
            //Pro_Steering();
            //Single_Pro_SteeringR();
        }

        /*if (myObjectCode == 2)
        {
            Single_Pro_SteeringL();
        }

        if (myObjectCode == 3)
        {
            Pro_Steering();
        }*/

        //Pro_Steering();

        WorkingCarControl();

        //Pro_Steering();

    }
}


void EllisCarControl()
{
    //  analogWrite(D4, mySpeed);
    // ei_printf("D5 at: %3d\n", myGlobalD5);
    //ThisThread::sleep_for(10);
}


void BufferControl()
{

    Buffer();

    if ((myPreviousObject == 0) && (myPastObject == 0)) 
    {
        myObjectCode = 0;
        //Foward(mySpeed);
        //Steering_Straight();
        //Steering('S');
        //myCarDirection = 'S';
    }
    else if ((myPreviousObject == 1) && (myPastObject == 1))
    {
        myObjectCode = 1;
        //Foward(mySpeed);
        //Steering_Right();
        //Steering('R');
        //myCarDirection = 'R';
    }
    else if ((myPreviousObject == 2) && (myPastObject == 2))
    {
        myObjectCode = 2;
        //Foward(mySpeed);
        //Steering_Left();
        //Steering('L');
        //myCarDirection = 'L';
    }
    else if ((myPreviousObject == 3) && (myPastObject == 3))
    {
        myObjectCode = 3;
        //Foward(mySpeed);
        //Steering_Straight();
        //Steering('S');
        //myCarDirection = 'S';
    }
    else
    {
        Coast();
    }
}


int currentMillis = millis();
int TimeOUT = 250;
int pastMillis = currentMillis;

void WorkingCarControl()
{

    if (myObjectCode == 0)
    {
        Brake();
        Steering_Straight();
    }
    /*else if (myObjectCode == 1)
    {
        Foward(mySpeed);
        //    Steering_Right();
    }//*/
    else if (myObjectCode == 1)
    {
        Foward(/*mySpeed*/);
        //    Steering_Left();
    }
    /*else if (myObjectCode == 1)
    {

        int myFastSpeed = (mySlowSpeed / 2) + mySlowSpeed+1;

        if (mySpeed < myFastSpeed)
        {
        currentMillis = millis();
        }

        if ((currentMillis - pastMillis) >= TimeOUT)
        {

            //mySpeed++;// = mySpeed + 1;
            mySpeed = constrain(mySpeed, mySlowSpeed, myFastSpeed);
            Foward(mySpeed);
            pastMillis = currentMillis;
            mySpeed++;

        }

    }
    //    Steering_Straight();
  }//*/
    else
    {
        Coast();
    }

}
