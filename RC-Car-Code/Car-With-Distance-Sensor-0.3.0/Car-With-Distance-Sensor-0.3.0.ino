
/*
   Base RC car FOMO 96x96 with DC motor and servo.

   NOTE: NO OLED, NO SERIAL PRINTING, JUST LED AND MOTOR RESPONSES

    To get more than 10 detections

   go into the edgeimpulse library you installed for this project find the file
   \src\model-parameters\model_metadata.h

   and change the line from
   #define EI_CLASSIFIER_OBJECT_DETECTION_COUNT       10
   to
   #define EI_CLASSIFIER_OBJECT_DETECTION_COUNT       30
   another reference here
   https://learn.adafruit.com/adafruit-gfx-graphics-library/graphics-primitives

   Note1: Should work with any Edge Impulse model just change the below include to your model name

   IF getting TFLITE arena allocation errors
   AppData\Local\Arduino15\packages\arduino\hardware\mbed_portenta\3.0.1
   copy the boards.txt file, rename to boards.local.txt with just the followoing line.
  envie_m7.build.extra_flags=-DEI_CLASSIFIER_ALLOCATION_STATIC

*/

/* Includes ---------------------------------------------------------------- */

// this is the Arduino library downloaded from edge Impulse
//#include <ei-v8-1-1-jeremy-rc-car-1red-2white-cup-fomo-96x96_inferencing.h>

/* FOR the GRAYSCALE Waveshare OLED 128 x 128 using library Adafruit_SSD1327.h
      yellow (sck) D9
      blue  DIN (mosi) D8
      orange (cs) D7
      green (dc)  D6
      white (reset) not needed but D14 if you did

*/


//////PIN SET
#define SERVO_PIN               D2        //PIN FOR SERVO SIGNAL
#define CONTROL_PIN             D5        //PIN FOR ESC SIGNAL
#define REVERSE_PIN             D3
#define FORWARD_PIN             D1


//////STEERING ANGLES WORKS BEST WITH +-30 OF MID_POINT (TRYING TO MAKE THIS MORE INCLUSIVE TO MORE THAN JUST +-30) 
#define MID_POINT               93        //STRAIGHT FOR SERVO
#define LOW_POINT               63        //LEFT FOR SERVO (MAX)
#define HIGH_POINT              123       //RIGHT FOR SERVO (MAX)
#define VIEW_AREA               30        //ONLY FOR THE STEERING!!! HOW FAR OFF CENTRE WE WANT TO READ BEFORE REACHING MAX TURING ANGLE



//////MOTOR SPEEDS
#define MIN_SPEED               40        //THE ABSOLUTE LOWEST SPEED WE CAN AS TO NOT HAVE THE MOTOR HUM (DEFUALT 50)
#define SLOW_SPEED              45        //OUR LOWEST SPEED WE WANT TO GO
#define FAST_SPEED              45        //OUR FASTEST SPEED WE WANT TO GO (WONT EXCEED 255)
#define SPEED_INCREASE          1         //AT WHICH INCREMENT DO WE WANT TO INCREASE OUR SPEED
#define SPEED_INCREASE_DELAY    250       //HOW LONG WE WANT TO WAIT IN BETWEEN INCREASES 



//////COMMENT AND UNCOMMENT THESE AS NECESSARY (USED LIKE SWITCHES (HINT HINT)) 
#define ONE_OBJECT                        //ONLY LOOKING FOR ONE OBJECT FOR STEERING
//#define TWO_OBJECT                        //ONLY LOOKING FOR TWO OBJECTS FOR STEERING (OVERRIDES ONE_OBJECT)
#define USE_STRAIGHT                      //WHEN THE CAR SEES NOTHING PUT THE TIRES BACK TO STRAIGHT
#define EXT_STEERING                      //USE EXTERNAL STEERING FUNCTIONS
//#define LINE_OUT_STEERING                 //OUTPUT THE ANGLE OF STEERING TO SERIAL
#define EXT_DRIVING                       //USE EXTERNAL DRIVING FUNCTIONS
//#define LINE_OUT_DRIVING                  //OUT THE SPEED (0-255) OF THE MOTOR TO SERIAL
#define USE_BRAKE                         //IF WE SHOULD USE BRAKE WHEN NO OBJECT DETECTED
//#define USE_COAST                         //IF WE SHOULD USE COAST WHEN NO OBJECT DETECTED (OVERRIDES USE_BRAKE)
#define COLORS                            //IF WE SHOULD DISPLAY WHAT WE SHOULD BE DOING WITH THE ON BOARD RGB LED
//#define MBED                              //CONTROLS USING NAMESPACE MBED
#define RTOS                              //CONTROLS USING NAMESPACE RTOS
//#define SLEEP 0                           //WHAT SLEEP DURATION (MS) TO USE FOR EI_SLEEP (IF COMMENTED OUT OR DELETED WILL DEFUALT TO 1MS) 


//This is for defining the pins for the distance sensor.
#define TRIGGER_PIN 8  // Define the trigger pin
#define ECHO_PIN 9    // Define the echo pin
#define MAX_DISTANCE 200  // Maximum distance in centimeters


//////EDGE IMPULE INCLUDE FILES GO HEAR (ONLY ONE FILE CAN BE INCLUDED)

//#include <ei-FOMO-Brandon-redcL1-tpR2_inferencing.h>
//#include <ei-FOMO-Brandon-redcL1-tpR2-2_inferencing.h>    //diff weight 10
//#include <ei-Andrew-3D-Printed-Symbol-FOMO_inferencing.h> //OLD MODEL SEPT 29 2022
//#include <ei-Andrew-3D-Printed-Symbol-FOMO5_inferencing.h> //UPDATED MODEL DEC 1 2022

//My Libraries
//#include <ei-Andrew-3D-Printed-Symbol-FOMO_inferencing.h>
//#include <ei-3d-print-01-model-45Degree_inferencing.h>
#include <ei-3d-print-03-model-45Degree_inferencing.h>

//////END OF EDGE IMPULSE INCLUDE FILES



//////MUST HAVE!!!! CONTAINS ALL THE SETUP FOR THE CAR
#include <ML_CAR.h>

//int mySlowSpeed = 30;

Thread myDrivingThread;

//int mySpeed;// = mySlowSpeed; // the Big Motor PWM speed

//Servo Steering_servo;

bool myLoRaStop = false;

//int myObjectCode = 0;   // 0=unknown,   1= pop,  2= water,     ,  3  both pop and water  now 1 red cup upside down and 2 right side up white cup
int myMax1Y;
int myMax2Y;
int myObjectCount = 0;
int myObjectCodeNew = 0;
int myObjectCodeNext = 0;


const float MY_FOMO_CUTOFF = 0.85;

int myObjectCode = 0;



bool ShouldStop = false;




/**
  @brief      Arduino setup function
*/
void setup() {

  myDrivingThread.start(DrivingController_myDrivingThread);
  // put your setup code here, to run once:
  Serial.begin(115200);

  //This is for the distance Sensor.
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  

  INIT_SERVO();
  INIT_MOTOR();
  PIN_SETUP();

  //Pin_Set();


#ifdef EI_CAMERA_FRAME_BUFFER_SDRAM
  // initialise the SDRAM
  SDRAM.begin(SDRAM_START_ADDRESS);
#endif

  if (ei_camera_init()) {
    // Serial.println("Failed to initialize Camera!");
  }
  else {
    // Serial.println("Camera initialized");
  }

  for (size_t ix = 0; ix < ei_dsp_blocks_size; ix++) {
    ei_model_dsp_t block = ei_dsp_blocks[ix];
    if (block.extract_fn == &extract_image_features) {
      ei_dsp_config_image_t config = *((ei_dsp_config_image_t*)block.config);
      int16_t channel_count = strcmp(config.channels, "Grayscale") == 0 ? 1 : 3;
      if (channel_count == 3) {
        Serial.println("WARN: You've deployed a color model, but the Arduino Portenta H7 only has a monochrome image sensor. Set your DSP block to 'Grayscale' for best performance.");
        break; // only print this once
      }
    }
  }

}

/**
  @brief      Get data and run inferencing

  @param[in]  debug  Get debug info if true
*/
void loop()
{
  ShouldStop = false;
  
    if (ShouldStop == true)
    {
        //Brake();

        analogWrite(CONTROL_PIN, 0); // set Speed to Zero

        digitalWrite(D5, 1); // set one direction
        digitalWrite(D3, 1); // set one direction
          
        Serial.print("Brake!!");

        delay(5000);

        ShouldStop = false;
    }
    else
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

}   // end main loop
