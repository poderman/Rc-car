
//This is for meachine learning to drive by its self
void Driving()
{
  
  //digitalWrite(D15, HIGH);

  // instead of wait_ms, we'll wait on the signal, this allows threads to cancel us...
  if (ei_sleep(EI_SLEEP_DELAY) != EI_IMPULSE_OK) {
    return;
  }


  ei::signal_t signal;
  signal.total_length = EI_CLASSIFIER_INPUT_WIDTH * EI_CLASSIFIER_INPUT_HEIGHT;
  signal.get_data = &ei_camera_cutout_get_data;

  if (ei_camera_capture((size_t)EI_CLASSIFIER_INPUT_WIDTH, (size_t)EI_CLASSIFIER_INPUT_HEIGHT, NULL) == false) {
    Serial.println("Failed to capture image\r\n");
    return;
  }


  // set values before the loop
  //myObjectCode = 0;   // 0=unknown,   1= upside down red cup, 2 right side up white cup or toilet paper
  myMax1Y = -1;
  myMax2Y = -1;
  myObjectCount = 0;
  //myObjectCodeNew = 0;
  //myObjectCodeNext = 0;



  // Run the classifier
  ei_impulse_result_t result = { 0 };

  EI_IMPULSE_ERROR err = run_classifier(&signal, &result, debug_nn);
  if (err != EI_IMPULSE_OK) {
    Serial.println("ERR: Failed to run classifier, error number: " + String(err));
    return;
  }
  // print the predictions

  // For complex prints best to run Edge Impulse ei_printf
  // ei_printf("(DSP: %d ms., Classification: %d ms., Anomaly: %d ms.)",
  //     result.timing.dsp, result.timing.classification, result.timing.anomaly);
  // ei_printf(": \n");




  // new code for FOMO bounding box edge impulse models
  // if no value in the first box then nothing in any of the boxes
  bool bb_found = result.bounding_boxes[0].value > 0;
  for (size_t ix = 0; ix < EI_CLASSIFIER_OBJECT_DETECTION_COUNT; ix++) {

    myObjectCount++;

    auto bb = result.bounding_boxes[ix];
    if (bb.value == 0) {
      continue;
    }

    // do some detection logic here

    if ((float)bb.value >= MY_FOMO_CUTOFF) {

      // sepecific to RC car Library and label  1popgoright 2watergoleft  might have to change these
      if (bb.label == "1") { // upside down red cup was pop
        ei_printf("1,");
        if (myMax1Y < (int)bb.y) {
          myMax1Y = (int)bb.y;
          myX1 = (int)bb.x;
          myWidth1 = (int)bb.width;
        }
        // ei_printf("-%u-",myMax1Y);
      }


      if (bb.label == "1") {  // right side up white cup / toilet paper roll was water bottle
        ei_printf("2,");
        if (myMax2Y < (int)bb.y) {
          myMax2Y = (int)bb.y;
          myX2 = (int)bb.x;
          myWidth2 = (int)bb.width;
        }
        //ei_printf("-%u-",myMax2Y);
      }

    }

    // Serial.print("Label: "+String(bb.label) + ", bb.y: " +String(bb.y) + ":::");


    //  ei_printf("    %s (", bb.label);
    //  ei_printf_float(bb.value);
    // ei_printf(") [ x: %u, y: %u, width: %u, height: %u ]\n", bb.x, bb.y, bb.width, bb.height);
  }

  if (!bb_found) {
    // ei_printf("    No objects found\n");
  }

  //Pro_Steering();

  //   how the above is done for classifying exported edge impulse models
  //   for (size_t ix = 0; ix < EI_CLASSIFIER_LABEL_COUNT; ix++) {
  //      ei_printf("    %s: %.5f\n", result.classification[ix].label, result.classification[ix].value);
  // }



#if EI_CLASSIFIER_HAS_ANOMALY == 1
  Serial.println("    anomaly score: " + String(result.anomaly, 5));
#endif

  digitalWrite(LEDB, HIGH);   //on board LED's are turned off by HIGH
  digitalWrite(LEDG, HIGH);
  digitalWrite(LEDR, HIGH);


  // Serial.print(", myMax1Y: "+String(myMax1Y) + ", myMax2Y: " +String(myMax2Y) + "+++");
  //Serial.print("\t 1Y: " + String(myMax1Y) + ",\t2Y: " + String(myMax2Y) + "\t");

  // more fuzzy logic here
  if (myMax1Y < 0 && myMax2Y < 0) {
    myObjectCodeNew = 0;
    myObjectCode = 0; // nothing
  }
  if (myMax1Y > 0 && myMax2Y > 0) {
    myObjectCodeNew = 1;
    myObjectCode = 1; // red cup
  }
  /* if (myMax1Y < 0 && myMax2Y > 0) {
     myObjectCodeNew = 2;
     myObjectCode = 2; // white cup / toilet paper roll
    }
    if (myMax1Y > 0 && myMax2Y > 0) {
     myObjectCodeNew = 3;
     myObjectCode = 3; // both cups
    }*/

  // no buffer code written yet
  // rc car responds to every classification

  ei_printf("\tCode: %u \t", myObjectCode);

  if (myObjectCode == 0) {   // 0 unknown do nothing
    //digitalWrite(LEDR, LOW);      // red stop
    //Steering('S');          // wheels straight
//    mySpeed = 0;                // stop the car
    // ei_printf("0: Unknown Stop: %u\n", myObjectCode);
    ei_printf("stop");
  }


  if (myObjectCode == 1) {      // red cup was pop: Go Right
    //digitalWrite(LEDB, LOW);   // Blue LED on
    //if (mySpeed < mySlowSpeed){
    //mySpeed = mySlowSpeed;     // car slow

    //}
    //Steering('R');     // go right
    // ei_printf("1: Red Cup Go right: %u\n", myObjectCode);
    ei_printf("right");
  }


  /*if (myObjectCode == 2) {     // white cup or toilet paper was water bottle go left
    digitalWrite(LEDG, LOW);   // Green LED on
    mySpeed = mySlowSpeed;           // car slow
    //Steering('L');      // go left
    // ei_printf("2: white cup go left: %u\n", myObjectCode);
    ei_printf("left");
    }


    if (myObjectCode == 3 ) {            // both detected
    digitalWrite(LEDB, LOW);          // blue and green = cyan
    digitalWrite(LEDG, LOW);
    mySpeed = mySlowSpeed;                  // slow
    //Steering('S');             // go straight
    //ei_printf("3: Both: %u\n", myObjectCode);
    ei_printf("straight");
    }*/



  ei_printf("\n");
}
