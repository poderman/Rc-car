//Version 0.2.3
/*
This version still has major issues. But the main issue is that the motor does
not work when using Promode.
 */
/*
This is a script for a self driving car. The scipt uses a Edge Impules model
trained on dots to follow and drive around a room.

I am curretly working on adding a backup mode to this code so that you can drive
the car with a remote control so that if the car gets stuck you can move it back
on track. I also am adding this feature because its a lot if fun to drive a RC car
around.

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


//These are for the 0.96 oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C


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

//This is for the IR Drive.
#include <IRremote.h>  // Include the IRremote library
#include <Arduino.h> // Only needed by https://platformio.org/

#include <Servo.h>  // Include the Servo library

//These are needed to use the 0.96 oled
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


//////MUST HAVE!!!! CONTAINS ALL THE SETUP FOR THE CAR
#include <ML_CAR.h>

//int mySlowSpeed = 30;

Thread myDrivingThread;

//int mySpeed;// = mySlowSpeed; // the Big Motor PWM speed

//Servo Steering_servo;

bool myLoRaStop = false;


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


//int myObjectCode = 0;   // 0=unknown,   1= pop,  2= water,     ,  3  both pop and water  now 1 red cup upside down and 2 right side up white cup
int myMax1Y;
int myMax2Y;
int myObjectCount = 0;
int myObjectCodeNew = 0;
int myObjectCodeNext = 0;


const float MY_FOMO_CUTOFF = 0.85;

int myObjectCode = 0;



//This is for the controller stuff.
int Promode_On = 2;                      //This is for desiding what mode to be in.

int CurrentSpeed = 0;
int CurrentAngel = 90;

const int Promode_Button = 7;
int Promode_ButtonState = 0;
int PreviousPromode_ButtonState = 0;

const int Promode_LED = 6;

//This is for the IR receiver module.
const int receiverPin = 10;  // Pin connected to the OUT pin of IR Receiver Module

IRrecv irReceiver(receiverPin);  // Create an instance of the IRrecv class
decode_results irResults;  // Create an instance of the decode_results structure

//This is for the Servo.
Servo servoMotor;  // Create a servo object
const int servoPin = 2;  // Pin connected to the servo motor



/**
  @brief      Arduino setup function
*/
void setup() {

  myDrivingThread.start(DrivingController_myDrivingThread);
  // put your setup code here, to run once:
  Serial.begin(115200);

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


  
    // Initialize the OLED display with the I2C address
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }

  // Clear the buffer
  display.clearDisplay();



    //This sets up all the buttons for the car.
    pinMode(Promode_Button, INPUT);

    //This sets up all the LEDs for the car.
    pinMode(Promode_LED, OUTPUT);

    //This is for setting up the IR receiver.
    irReceiver.enableIRIn();  // Enable the IR Receiver Module
    servoMotor.attach(servoPin);  // Attach the servo to the specified pin
    servoMotor.write(90);

}

/**
  @brief      Get data and run inferencing

  @param[in]  debug  Get debug info if true
*/
void loop()
{
    if (Promode_On == 1) //This is for running the remote control part of the vehicle.
    {/*
        Promode();*/
    }
    
    else if (Promode_On == 2) //This is for activating and running the AI driving.
    {    
        Driving();
    }



    Promode_ButtonState = digitalRead(Promode_Button);

    // If the button is pressed (LOW state), perform an action
    if (Promode_ButtonState == LOW && PreviousPromode_ButtonState == HIGH)
    {
        if (Promode_On == 1)
        {
            Promode_On = 2;
            digitalWrite(Promode_LED, LOW);
            //analogWrite(D5, 0);   // Stop!!
        }
        else if (Promode_On == 2)
        {
            Promode_On = 1;
            digitalWrite(Promode_LED, HIGH);
            //analogWrite(D5, 0);   // Stop!!
        }
    }
    PreviousPromode_ButtonState = Promode_ButtonState;


    RunOled();
    
}   // end main loop
