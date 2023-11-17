
//This is for running the distance sensor to stop the car if it runs into a wall!!


void CheckDistance()
{
    // Send a trigger pulse to the sensor
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);

    // Measure the duration of the echo pulse
    long duration = pulseIn(ECHO_PIN, HIGH);

    // Calculate the distance based on the speed of sound
    float distance = (duration / 2) * 0.0344;  // Speed of sound in cm/ms

    if (distance >= 2 && distance <= MAX_DISTANCE)
    {
        Serial.print("Distance: ");
        Serial.print(distance);
        Serial.println(" cm");

        if (distance <= 15)
        {
          ShouldStop = true;
          
        }
        else
        {
          ShouldStop = false;
        }
        
    } 
    else 
    {
        Serial.println("Out of range");
    }
}
