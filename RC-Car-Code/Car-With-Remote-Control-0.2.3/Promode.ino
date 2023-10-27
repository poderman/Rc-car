
//This is for running Promode
void Promode()
{
    if(irReceiver.decode())
    {
        if (irReceiver.decodedIRData.decodedRawData == 0x50B87EE) //Go forward.
        {
            CurrentAngel = 90;
            servoMotor.write(CurrentAngel);
        
            digitalWrite(D1, 1);    // set one direction
            digitalWrite(D3, 0);    // set one direction 
    
            CurrentSpeed = 35;
            analogWrite(D5, 35);   // go medium
        }
        else if (irReceiver.decodedIRData.decodedRawData == 0x50D87EE) //Go backwards.
        {
            CurrentAngel = 90;
            servoMotor.write(CurrentAngel);

            digitalWrite(D1, 0);    // set one direction
            digitalWrite(D3, 1);    // set one direction
    
            CurrentSpeed = 35;
            analogWrite(D5, 35);   // go medium
        }
        else if (irReceiver.decodedIRData.decodedRawData == 0x50887EE) //Go left
        {
            CurrentAngel = 65;
            servoMotor.write(CurrentAngel);

            digitalWrite(D1, 1);    // set one direction
            digitalWrite(D3, 0);    // set one direction 
    
            CurrentSpeed = 35;
            analogWrite(D5, 35);   // go medium
        }
        else if (irReceiver.decodedIRData.decodedRawData == 0x50787EE) //Go right
        {
            CurrentAngel = 115;
            servoMotor.write(CurrentAngel);

            digitalWrite(D1, 1);    // set one direction
            digitalWrite(D3, 0);    // set one direction 
    
            CurrentSpeed = 35;
            analogWrite(D5, 35);   // go medium
        }
        else if (irReceiver.decodedIRData.decodedRawData == 0x55E87EE) //Stop!!
        {
            CurrentAngel = 90;
            servoMotor.write(CurrentAngel);

            digitalWrite(D1, 1);    // set one direction
            digitalWrite(D3, 1);    // set one direction 
    
            CurrentSpeed = 0;
            analogWrite(D5, 0);   // Stop
        }
    }
    irReceiver.resume();  // Enable the IR Receiver to receive the next code
}
