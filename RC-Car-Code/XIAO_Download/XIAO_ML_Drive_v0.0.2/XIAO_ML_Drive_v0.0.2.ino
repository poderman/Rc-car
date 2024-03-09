/*  This project was made by William Jacobs.
 * 
 *  Version0.0.2
 *  This version can find objects using the XIAO camera and print what to do in
 *  the Serial Monitor. Next I need to get this working with the car and its motors
 *  and then maybe to some complex math to be better at driving.
 *
 */
/* Includes ---------------------------------------------------------------- */

//This is for the meachine learning stuff.
//////PIN SET
#define SERVO_PIN               D0        //PIN FOR SERVO SIGNAL
#define CONTROL_PIN             D2        //PIN FOR ESC SIGNAL
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
#define FAST_SPEED              40        //OUR FASTEST SPEED WE WANT TO GO (WONT EXCEED 255)
#define SPEED_INCREASE          1         //AT WHICH INCREMENT DO WE WANT TO INCREASE OUR SPEED
#define SPEED_INCREASE_DELAY    250       //HOW LONG WE WANT TO WAIT IN BETWEEN INCREASES 


///////////////Include Edge Impulse model goes here//////////////////
#include <ei-3d-print-03-model-45Degree_inferencing.h>
#include "edge-impulse-sdk/dsp/image/image.hpp"

#include "esp_camera.h"


// Define camera model 
#define PWDN_GPIO_NUM     -1 
#define RESET_GPIO_NUM    -1 
#define XCLK_GPIO_NUM     10 
#define SIOD_GPIO_NUM     40 
#define SIOC_GPIO_NUM     39
#define Y9_GPIO_NUM       48 
#define Y8_GPIO_NUM       11 
#define Y7_GPIO_NUM       12 
#define Y6_GPIO_NUM       14 
#define Y5_GPIO_NUM       16 
#define Y4_GPIO_NUM       18 
#define Y3_GPIO_NUM       17 
#define Y2_GPIO_NUM       15 
#define VSYNC_GPIO_NUM    38 
#define HREF_GPIO_NUM     47 
#define PCLK_GPIO_NUM     13

/* Constant defines -------------------------------------------------------- */
#define EI_CAMERA_RAW_FRAME_BUFFER_COLS           320
#define EI_CAMERA_RAW_FRAME_BUFFER_ROWS           240
#define EI_CAMERA_FRAME_BYTE_SIZE                 3

/* Private variables ------------------------------------------------------- */
static bool debug_nn = false; // Set this to true to see e.g. features generated from the raw signal
static bool is_initialised = false;
uint8_t *snapshot_buf; //points to the output of the capture

static camera_config_t camera_config = {
    .pin_pwdn = PWDN_GPIO_NUM,
    .pin_reset = RESET_GPIO_NUM,
    .pin_xclk = XCLK_GPIO_NUM,
    .pin_sscb_sda = SIOD_GPIO_NUM,
    .pin_sscb_scl = SIOC_GPIO_NUM,

    .pin_d7 = Y9_GPIO_NUM,
    .pin_d6 = Y8_GPIO_NUM,
    .pin_d5 = Y7_GPIO_NUM,
    .pin_d4 = Y6_GPIO_NUM,
    .pin_d3 = Y5_GPIO_NUM,
    .pin_d2 = Y4_GPIO_NUM,
    .pin_d1 = Y3_GPIO_NUM,
    .pin_d0 = Y2_GPIO_NUM,
    .pin_vsync = VSYNC_GPIO_NUM,
    .pin_href = HREF_GPIO_NUM,
    .pin_pclk = PCLK_GPIO_NUM,

    //XCLK 20MHz or 10MHz for OV2640 double FPS (Experimental)
    .xclk_freq_hz = 20000000,
    .ledc_timer = LEDC_TIMER_0,
    .ledc_channel = LEDC_CHANNEL_0,

    .pixel_format = PIXFORMAT_JPEG, //YUV422,GRAYSCALE,RGB565,JPEG
    .frame_size = FRAMESIZE_QVGA,    //QQVGA-UXGA Do not use sizes above QVGA when not JPEG

    .jpeg_quality = 12, //0-63 lower number means higher quality
    .fb_count = 1,       //if more than one, i2s runs in continuous mode. Use only with JPEG
    .fb_location = CAMERA_FB_IN_PSRAM,
    .grab_mode = CAMERA_GRAB_WHEN_EMPTY,
};

/* Function definitions ------------------------------------------------------- */
bool ei_camera_init(void);
void ei_camera_deinit(void);
bool ei_camera_capture(uint32_t img_width, uint32_t img_height, uint8_t *out_buf) ;


/*
//These are for the 0.96 oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C


//These are needed to use the 0.96 oled
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int myObjectCode = 0;
*/

//This is for the IR Drive.
#include <IRremote.h>  // Include the IRremote library
#include <Arduino.h> // Only needed by https://platformio.org/

#include <ESP32Servo.h>  // This is a special Servo library for ESP32 boards


//This is for the IR receiver module.
const int receiverPin = 8;  // Pin connected to the OUT pin of IR Receiver Module

IRrecv irReceiver(receiverPin);  // Create an instance of the IRrecv class
decode_results irResults;  // Create an instance of the decode_results structure

//This is for the Servo.
Servo servoMotor;  // Create a servo object
const int servoPin = 0;  // Pin connected to the servo motor


//This is for the controller stuff.
int CurrentMode = 2;    //This is what the current mode is __ 1 = IR_Remote, 2 = Tiny ML

int CurrentSpeed = 0;
int CurrentAngel = 90;

const int SwitchMode_Button = 7;
int SwitchMode_ButtonState = 0;
int PreviousPromode_ButtonState = 0;

const int CurrentMode_LED = 6;


/**
* @brief      Arduino setup function
*/
void setup()
{
  
    // put your setup code here, to run once:
    Serial.begin(115200);
    //comment out the below line to start inference immediately after upload
    while (!Serial);
    Serial.println("Edge Impulse Inferencing Demo");
    if (ei_camera_init() == false) {
        ei_printf("Failed to initialize Camera!\r\n");
    }
    else {
        ei_printf("Camera initialized\r\n");
    }

/*
    //Pin_Set();

    // Initialize the OLED display with the I2C address
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }
    

    // Clear the buffer
    display.clearDisplay();
*/


    //This sets up all the buttons for the car.
    pinMode(SwitchMode_Button, INPUT);

    //This sets up all the LEDs for the car.
    pinMode(CurrentMode_LED, OUTPUT);

    //This is for setting up the IR receiver.
    irReceiver.enableIRIn();  // Enable the IR Receiver Module
    servoMotor.attach(servoPin);  // Attach the servo to the specified pin
    servoMotor.write(90);

    ei_printf("\nStarting continious inference in 2 seconds...\n");
    ei_sleep(2000);

}


void loop()
{

    if (CurrentMode == 1) //This is for running the remote control part of the vehicle.
    {
        IR_Remote();
    }
    
    else if (CurrentMode == 2) //This is for activating and running the AI driving.
    {    
        ML_Driving();
    }



    SwitchMode_ButtonState = digitalRead(SwitchMode_Button);

    // If the button is pressed (LOW state), perform an action
    if (SwitchMode_ButtonState == LOW && PreviousPromode_ButtonState == HIGH)
    {
        if (CurrentMode == 1)
        {
            CurrentMode = 2;
            digitalWrite(CurrentMode_LED, LOW);
            //analogWrite(D5, 0);   // Stop!!
        }
        else if (CurrentMode == 2)
        {
            CurrentMode = 1;
            digitalWrite(CurrentMode_LED, HIGH);
            //analogWrite(D5, 0);   // Stop!!
        }
    }
    PreviousPromode_ButtonState = SwitchMode_ButtonState;


    //RunOled();

    
}   // end main loop
