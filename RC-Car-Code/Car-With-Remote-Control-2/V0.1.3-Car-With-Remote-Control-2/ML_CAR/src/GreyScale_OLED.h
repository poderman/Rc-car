#ifndef USE_OLED
#define USE_OLED

#include <Adafruit_SSD1327.h>
#include <map>






// Used for software SPI
#define OLED_CLK D9
#define OLED_MOSI D8

// Used for software or hardware SPI
#define OLED_CS D7
#define OLED_DC D6

// Used for I2C or SPI
#define OLED_RESET -1

// hardware SPI
Adafruit_SSD1327 display(128, 128, &SPI, OLED_DC, OLED_RESET, OLED_CS);

int x1Map, x2Map, y1Map, y2Map;



#define CUTOUT_COLS                 EI_CLASSIFIER_INPUT_WIDTH
#define CUTOUT_ROWS                 EI_CLASSIFIER_INPUT_HEIGHT
const int cutout_row_start = (EI_CAMERA_RAW_FRAME_BUFFER_ROWS - CUTOUT_ROWS) / 2;
const int cutout_col_start = (EI_CAMERA_RAW_FRAME_BUFFER_COLS - CUTOUT_COLS) / 2;


int myTotalObjects = 0;
std::map<std::string, int> cnt;

auto Color = SSD1327_WHITE;

int myObjectCode = 0;


ei_impulse_result_t result = { 0 };

size_t ix = 0;

auto bb = result.bounding_boxes[ix];

const float MY_FOMO_CUTOFF = 0.85;






void Setup_OLED() {

  if ( ! display.begin(0x3D) ) {   // start Grayscale OLED
    Serial.println("Unable to initialize OLED");
    while (1) yield();
  }
  display.setTextSize(1);
  display.setTextColor(SSD1327_WHITE);

  display.setRotation(2);
  display.setCursor(0, 0);

  //   map cutout of the 320 x 320   // 240 model to OLED 128 x 64 screen
  x1Map = map((int)cutout_col_start, 0, 320, 0, 127);
  x2Map = map((int)CUTOUT_COLS, 0, 320, 0, 127);
  y1Map = map((int)cutout_row_start, 0, 320, 0, 127);
  y2Map = map((int)CUTOUT_ROWS, 0, 320, 0, 127);
  
}



void OLED_init() {

  // Put the image on the OLED
  display.clearDisplay();                 // clear the internal memory for OLED
  for (int x = 0; x < EI_CAMERA_RAW_FRAME_BUFFER_COLS; x++) {  // EI_CAMERA_RAW_FRAME_BUFFER_COLS = 320
    for (int y = 0; y < EI_CAMERA_RAW_FRAME_BUFFER_ROWS; y++) {    //EI_CAMERA_RAW_FRAME_BUFFER_ROWS = 320   //240

      uint8_t myGRAY = ei_camera_frame_buffer[(y * (int)EI_CAMERA_RAW_FRAME_BUFFER_COLS) + x];

      int myGrayMap = map(myGRAY, 0, 255, 0, 15);
      int xMap = map(x, 0, 320, 0, 128);
      int yMap = map(y, 0, 320, 0, 128);
      display.drawPixel(xMap, yMap, myGrayMap);   // grayscale 0-255, 128x128  //128 x 64
    }
  }

  display.drawRect(1, 1,   126, 126, SSD1327_WHITE );  // rectangle around outside of OLED

  display.setCursor(20, 5);
  display.println("Rocksetta: ");
}

void M_OLED() {

  std::string label(bb.label);
  auto cnt_entry = cnt.find(label);
  if (cnt_entry == cnt.end()) {
    cnt[label] = 1;
  }
  else {
    cnt_entry->second++;
  }

  int xMap = map(bb.x, 0, 96, 0, 128);
  int yMap = map(bb.y, 0, 96, 0, 128);
  int widthMap = map(bb.width, 0, 96, 0, 128);
  int heightMap = map(bb.height, 0, 96, 0, 128);

  if ((float)bb.value >= MY_FOMO_CUTOFF) {

    Color = SSD1327_WHITE;
    display.drawRect(xMap, yMap, widthMap, heightMap, Color );
  } else {
    
    Color = SSD1327_BLACK;
    display.fillRect(xMap, yMap, widthMap, heightMap, Color );
  }

  //display.drawRect(xMap, yMap, widthMap, heightMap, Color );
  myTotalObjects += EI_CLASSIFIER_OBJECT_DETECTION_COUNT;
  display.setCursor(xMap, yMap - 5);
  display.println("x:" + String(xMap) + ",y:" + String(yMap)); // label

}

void MM_OLED() {

  display.setCursor(bb.x - 3, bb.y - 2);
  String myShortLabel = String(bb.label).substring(0, 4);
  display.println(String(myShortLabel) );  // only print the start of the label

}


void MMM_OLED() {

  int myLoop = 2;
  for (auto it : cnt) {
    // printf("%s: %d\n", it.first.c_str(), it.second);
    //display.setCursor(80,5); // near the top
    //display.println(String(myTotalObjects));
    myLoop++;
    display.setCursor(25, (myLoop * 10));
    display.println(it.first.c_str());  // label
    display.setCursor(5, (myLoop * 10));
    display.println(String(it.second)); // count for that label
  }

}

void MMMM_OLED() {

  if (myObjectCode == 0) {

    display.setCursor(3, 110);
    display.println("0: UNKNOWN /STOP/: " + String(myObjectCode));
  }

  if (myObjectCode == 1) {

    display.setCursor(3, 110);
    display.println("1: RED CUP - GO RIGHT: " + String(myObjectCode));

  }

  if (myObjectCode == 2) {

    display.setCursor(3, 110);
    display.println("2: TOILET PAPER - GO LEFT: " + String(myObjectCode));

  }

  if (myObjectCode == 3) {

    display.setCursor(3, 110);
    display.println("3: TP & RC - STRAIGHT: " + String(myObjectCode));

  }
}

void MMMMM_OLED() {

  display.setCursor(80, 5);
  display.println(String(myTotalObjects));

  display.display();
}


#endif