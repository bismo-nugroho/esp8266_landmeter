/*
  Created by Bodmer 1/12/17

  #########################################################################
  ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
  #########################################################################
*/

#define BACKGROUND TFT_BLACK

// Pause in milliseconds to set refresh speed
#define WAIT 20

#include <TFT_eSPI.h>                 // Include the graphics library (this includes the sprite functions)


#include <Wire.h>              // Wire library (required for I2C devices)
#include <Adafruit_BMP280.h>   // Adafruit BMP280 sensor library
#include <Adafruit_MPU6050.h>
//#include <MPU6050_light.h>
#include <Adafruit_Sensor.h>

#define BMP280_I2C_ADDRESS  0x76
#include "alert.h"
#include "back.h"

TFT_eSPI    tft = TFT_eSPI();         // Create object "tft"

TFT_eSprite img = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite imgs = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite sc1 = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite sc2 = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite sc3 = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite sc4 = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT

TFT_eSprite ft = TFT_eSprite(&tft);  // Create Sprite object "img" with pointer to "tft" object
// the pointer is used by pushSprite() to push it onto the TFT


int number = 0;
int angle  = 0;
int angles  = 0;

int lx1 = 0;
int ly1 = 0;
int lx2 = 0;
int ly2 = 0;
int lx3 = 0;
int ly3 = 0;
int lx4 = 0;
int ly4 = 0;


int lxs1 = 0;
int lys1 = 0;
int lxs2 = 0;
int lys2 = 0;
int lxs3 = 0;
int lys3 = 0;
int lxs4 = 0;
int lys4 = 0;

// Test only
uint16_t  n = 0;
uint32_t dt = 0;

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0;    // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 120, omx = 120, omy = 120, ohx = 120, ohy = 120; // Saved H, M, S x & y coords
uint16_t x0 = 0, x1 = 0, x2 = 0, x3 = 0, yy0 = 0, yy1 = 0, yy2 = 0, yy3 = 0;
uint32_t targetTime = 0;                    // for next 1 second timeout

//static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
//uint8_t hh=conv2d(__TIME__), mm=conv2d(__TIME__+3), ss=conv2d(__TIME__+6);  // Get H, M, S from compile time

bool initial = 1;

// Palette colour table
uint16_t palette[16];

Adafruit_BMP280  bmp280;

Adafruit_MPU6050 mpu;

//MPU6050 mpu(Wire);

#define GREEN 9
#define YELLOW 11
#define RED 10
#define BLACK 6
#define WHITE 15
#define BLUE 9

// -------------------------------------------------------------------------
// Setup
// -------------------------------------------------------------------------
void setup(void) {
  Serial.begin(9600);

  // Populate the palette table, table must have 16 entries
  palette[0]  = TFT_BLACK;
  palette[1]  = TFT_ORANGE;
  palette[2]  = TFT_DARKGREEN;
  palette[3]  = TFT_DARKCYAN;
  palette[4]  = TFT_MAROON;
  palette[5]  = TFT_PURPLE;
  palette[6]  = TFT_OLIVE;
  palette[7]  = TFT_DARKGREY;
  palette[8]  = TFT_ORANGE;
  palette[9]  = TFT_BLUE;
  palette[10] = TFT_GREEN;
  palette[11] = TFT_CYAN;
  palette[12] = TFT_RED;
  palette[13] = TFT_NAVY;
  palette[14] = TFT_YELLOW;
  palette[15] = TFT_WHITE;

  tft.init();

  tft.setRotation(1);

  tft.fillScreen(BACKGROUND);

  /* Default settings from datasheet. */
  bmp280.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                     Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                     Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                     Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                     Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */

  Wire.begin(D2, D1);  // set I2C pins [SDA = D2, SCL = D1], default clock is 100kHz
  //Wire.begin();
  if ( bmp280.begin(BMP280_I2C_ADDRESS) == 0 ) {
    // connection error or device address wrong!
    //tft.setTextColor(ST77XX_WHITE, ST77XX_BLACK);   // set text color to white and black background
    tft.setTextSize(10);      // text size = 4
    tft.setCursor(3, 88);    // move cursor to position (3, 88) pixel
    tft.print("Connection");
    tft.setCursor(63, 126);  // move cursor to position (63, 126) pixel
    tft.print("Error");
    while (1); // stay here
  }


  //  Wire.begin();
  //  mpu.begin();
  // display.println(F("Calculating gyro offset, do not move MPU6050"));
  // display.display();
  // mpu.calcGyroOffsets();


  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  Serial.println("MPU6050 Found!");
  ///*
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
    case MPU6050_RANGE_2_G:
      Serial.println("+-2G");
      break;
    case MPU6050_RANGE_4_G:
      Serial.println("+-4G");
      break;
    case MPU6050_RANGE_8_G:
      Serial.println("+-8G");
      break;
    case MPU6050_RANGE_16_G:
      Serial.println("+-16G");
      break;
  }
  mpu.setGyroRange(MPU6050_RANGE_2000_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
    case MPU6050_RANGE_250_DEG:
      Serial.println("+- 250 deg/s");
      break;
    case MPU6050_RANGE_500_DEG:
      Serial.println("+- 500 deg/s");
      break;
    case MPU6050_RANGE_1000_DEG:
      Serial.println("+- 1000 deg/s");
      break;
    case MPU6050_RANGE_2000_DEG:
      Serial.println("+- 2000 deg/s");
      break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_5_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
    case MPU6050_BAND_260_HZ:
      Serial.println("260 Hz");
      break;
    case MPU6050_BAND_184_HZ:
      Serial.println("184 Hz");
      break;
    case MPU6050_BAND_94_HZ:
      Serial.println("94 Hz");
      break;
    case MPU6050_BAND_44_HZ:
      Serial.println("44 Hz");
      break;
    case MPU6050_BAND_21_HZ:
      Serial.println("21 Hz");
      break;
    case MPU6050_BAND_10_HZ:
      Serial.println("10 Hz");
      break;
    case MPU6050_BAND_5_HZ:
      Serial.println("5 Hz");
      break;
  }
  //*/

  //drawScale(120, 40);

  tft.setSwapBytes(true);
}

long timer = 0;
float temp     = 0;
float pressure = 0;
float altitude = 0;

long redrawData = 0;
int LV_DELAY = 100;
// -------------------------------------------------------------------------
// Main loop
// -------------------------------------------------------------------------'


int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

int minVal = 265;
int maxVal = 402;

double xax;
double yax;
double zax;


void loop() {

  if (millis() - timer > LV_DELAY) {

    timer = millis();
    // read temperature and pressure from BMP280 sensor
    temp     = bmp280.readTemperature();   // get temperature
    pressure = bmp280.readPressure();      // get pressure
    //float altitude = bmp280.readAltitude(1019.66);

    altitude = bmp280.readAltitude(1013.25);



    //mpu.update();
    sensors_event_t a, g, temps;
    mpu.getEvent(&a, &g, &temps);

    //angle+=3; //Increment angle for testing

    // if (angle > 359) angle = 0; // Limit angle to 360
    // }
    // angle = mpu.getAngleY();// - (int) ( a.acceleration.y * 10 );
    //angles = mpu.getAngleZ();// + (int) ( a.acceleration.z * 10 );

    //angle =  - (int) ( ( a.acceleration.y + g.gyro.y ) * 10 );
    //angles =  + (int) ( ( a.acceleration.z + g.gyro.z ) * 10 );

    // z = x




    int pitch = (atan2(a.acceleration.z, sqrt(a.acceleration.z * a.acceleration.z + a.acceleration.x * a.acceleration.x)) * 180.0) / M_PI;
    int roll = -(atan2(a.acceleration.y, a.acceleration.x) * 180.0) / M_PI;

    angles = pitch;
    angle = roll;

    //Serial.print("aCC x axis = "); Serial.println(a.acceleration.x);
    // Serial.print("acc y axis = "); Serial.println(a.acceleration.y);
    //Serial.print("acc z axis = "); Serial.println(a.acceleration.z);


    //Serial.print("pitch = "); Serial.println(pitch);
    // Serial.print("roll = "); Serial.println(roll);
    //Serial.print("z axis = "); Serial.println(zax);






    drawCompass(75, 70, 120, 40, angles); // Draw centre of compass at 50,50

    drawCompass2(75, 55, 120, 150, angle); // Draw centre of compass at 50,50

    drawScaleSprite(120, 40, angles);
    drawScaleSprite1(120, 40, angles);

    drawScaleSprites(120, 40, angle);
    drawScaleSprites1(120, 40, angle);
    drawFooterSprite(temp, altitude);
    /*

      tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Text with background
      //tft.setCursor(0, 210 );
      //tft.setTextDatum(BL_DATUM);
      //tft.printf("%s °c ", String(temps.temperature, 1));

      //tft.setCursor(158, 210 );
      tft.setTextPadding(50);
      tft.setTextDatum(BL_DATUM);
      //tft.drawNumber((int) altitude);
      //tft.printf( "%s m", String(altitude, 0) );
      // tft.drawString( String(temps.temperature, 1), 0, 235, 4);
      tft.drawString( String(temp, 1), 0, 235, 4);
      tft.setTextPadding(10);
      tft.drawString("c", 55, 235, 4);

      //tft.setCursor(158, 210 );
      tft.setTextPadding(50);
      tft.setTextDatum(BR_DATUM);
      //tft.drawNumber((int) altitude);
      //tft.printf( "%s m", String(altitude, 0) );
      tft.drawString( String(altitude, 0), 215, 235, 4);
      tft.setTextPadding(1);
      tft.drawString("m", 240, 235, 4);
      // tft.drawNumber(altitude);
      //}

      //delay(WAIT);
    */

    //delay(1000);
  }

}


// Test code to measure runtimes, executes code 100x and shows time taken
#define TSTART //n=100;dt=millis();while(n--){
#define TPRINT //};Serial.println((millis()-dt)/100.0);


void drawFooterSprite(float temp, float altitude) {


  // img.pushSprite(posx-x, posy-y, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  // img.deleteSprite();

  ft.setColorDepth(4);
  ft.createSprite(240, 30);
  ft.createPalette(palette);
    ft.fillSprite(BLACK);

  //ft.setTextColor(RED, BLACK); // Text with background
  //tft.setCursor(0, 210 );
  //tft.setTextDatum(BL_DATUM);
  //tft.printf("%s °c ", String(temps.temperature, 1));

  //tft.setCursor(158, 210 );
  // ft.setTextPadding(50);
  //ft.setTextDatum(BL_DATUM);
  //tft.drawNumber((int) altitude);
  //tft.printf( "%s m", String(altitude, 0) );
  // tft.drawString( String(temps.temperature, 1), 0, 235, 4);
  ft.drawString( String(temp, 1)+" c", 0, 0, 4);
  //ft.setTextPadding(10);
  // ft.drawString("c", 55, 0, 4);

  //tft.setCursor(158, 210 );
   ft.setTextPadding(70);
 // ft.setTextDatum(BR_DATUM);
  //tft.drawNumber((int) altitude);
  //tft.printf( "%s m", String(altitude, 0) );
  ft.drawString( String(altitude, 0)+" m", 170, 0, 4);
  // ft.setTextPadding(1);
  //ft.drawString("m", 240, 0, 4);


  ft.pushSprite(0, 210, BLACK);
  //ft.setPivot(120, 220);     // Set pivot to middle of TFT screen
  //ft.pushRotated(0);
  ft.deleteSprite();
}


void drawScaleSprite(int x, int y, int angle) {

  sc1.fillSprite(TFT_WHITE);
  // img.pushSprite(posx-x, posy-y, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  // img.deleteSprite();

  sc1.setColorDepth(4);
  sc1.createSprite(44, 80);
  sc1.createPalette(palette);

  int ang = 0;
  ang = 270 + angle;

  // Draw 12 lines

  // Draw 12 lines
  for (int i = 0 ; i < 360; i += 1) {
    if (  ( i >= 70 && i <= 110 ) ||
          ( i <= 290 && i >= 250 ) ) {

      sx = cos((i - 90) * 0.0174532925);
      sy = sin((i - 90) * 0.0174532925);
      x0 = sx * 114 + x;
      yy0 = sy * 114 + y;

      x1 = sx * 90 + x;
      yy1 = sy * 90 + y;

      x2 = sx * 100 + x;
      yy2 = sy * 100 + y;

      x3 = sx * 80 + x;
      yy3 = sy * 80 + y;


      if (ang >= 270 && i >= 270 ) {
        if (i <= ang) {
          if (i < 280) sc1.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i < 290 ) sc1.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc1.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (ang <= 270 && i < 270 ) {
        if (i >= ang) {
          if (i > 260) sc1.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i > 250 ) sc1.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc1.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (i == 270 || i == 90) sc1.drawLine(x0, yy0, x1, yy1, GREEN );
      else if (i == 280 || i == 80 || i == 100 || i == 260) sc1.drawLine(x0, yy0, x1, yy1, YELLOW);
      else if (i == 290 || i == 70 || i == 250 || i == 110 ) sc1.drawLine(x0, yy0, x1, yy1, RED);

      if (i % 5 == 0 && i % 10 != 0)  sc1.drawLine(x0, yy0, x2, yy2, 15);

      if (i == ang) sc1.drawLine(x0, yy0, x3, yy3, 15);

    }
  }

  //sc1.pushSprite(0, 0, TFT_TRANSPARENT);
  tft.setPivot(22, 40);     // Set pivot to middle of TFT screen
  sc1.pushRotated(0);
  sc1.deleteSprite();
}

void drawScaleSprite1(int x, int y, int angle) {

  sc2.fillSprite(TFT_BLACK);
  // img.pushSprite(posx-x, posy-y, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  // img.deleteSprite();

  sc2.setColorDepth(4);
  sc2.createSprite(44, 80);
  sc2.createPalette(palette);

  int ang = 0;
  ang = 270 + angle;
  /*
    sc1.pushRotated(&sc2,360,TFT_BLACK);
    sc2.pushSprite(240, 0, TFT_BLACK);
    //tft.setPivot(220, 40);     // Set pivot to middle of TFT screen
    //sc2.pushRotated(180);
    sc2.deleteSprite();
  */

  // Draw 12 lines
  for (int i = 0 ; i < 360; i += 1) {
    if (  ( i >= 70 && i <= 110 ) ||
          ( i <= 290 && i >= 250 ) ) {

      sx = cos((i - 90) * 0.0174532925);
      sy = sin((i - 90) * 0.0174532925);
      x0 = sx * 114 + x;
      yy0 = sy * 114 + y;

      x1 = sx * 90 + x;
      yy1 = sy * 90 + y;

      x2 = sx * 100 + x;
      yy2 = sy * 100 + y;

      x3 = sx * 80 + x;
      yy3 = sy * 80 + y;

      if (ang >= 270 && i >= 270 ) {
        if (i <= ang) {
          if (i < 280) sc2.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i < 290 ) sc2.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc2.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (ang <= 270 && i < 270 ) {
        if (i >= ang) {
          if (i > 260) sc2.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i > 250 ) sc2.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc2.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (i == 270 || i == 90) sc2.drawLine(x0, yy0, x1, yy1, GREEN );
      else if (i == 280 || i == 80 || i == 100 || i == 260) sc2.drawLine(x0, yy0, x1, yy1, YELLOW);
      else if (i == 290 || i == 70 || i == 250 || i == 110 ) sc2.drawLine(x0, yy0, x1, yy1, RED);

      if (i % 5 == 0 && i % 10 != 0)  sc2.drawLine(x0, yy0, x2, yy2, 15);

      if (i == ang) sc2.drawLine(x0, yy0, x3, yy3, 15);


    }
  }

  //sc1.pushSprite(0, 0, TFT_TRANSPARENT);
  tft.setPivot(218, 40);     // Set pivot to middle of TFT screen
  sc2.pushRotated(180);
  sc2.deleteSprite();
}

void drawScaleSprites(int x, int y, int angle) {

  sc3.fillSprite(TFT_WHITE);
  // img.pushSprite(posx-x, posy-y, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  // img.deleteSprite();

  sc3.setColorDepth(4);
  sc3.createSprite(44, 80);
  sc3.createPalette(palette);

  int ang = 0;
  ang = 270 + angle;


  // Draw 12 lines


  // Draw 12 lines
  for (int i = 0 ; i < 360; i += 1) {
    if (  ( i >= 70 && i <= 110 ) ||
          ( i <= 290 && i >= 250 ) ) {

      sx = cos((i - 90) * 0.0174532925);
      sy = sin((i - 90) * 0.0174532925);
      x0 = sx * 114 + x;
      yy0 = sy * 114 + y;

      x1 = sx * 90 + x;
      yy1 = sy * 90 + y;

      x2 = sx * 100 + x;
      yy2 = sy * 100 + y;

      x3 = sx * 80 + x;
      yy3 = sy * 80 + y;

      if (ang >= 270 && i >= 270 ) {
        if (i <= ang) {
          if (i < 280) sc3.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i < 290 ) sc3.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc3.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (ang <= 270 && i < 270 ) {
        if (i >= ang) {
          if (i > 260) sc3.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i > 250 ) sc3.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc3.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (i == 270 || i == 90) sc3.drawLine(x0, yy0, x1, yy1, GREEN );
      else if (i == 280 || i == 80 || i == 100 || i == 260) sc3.drawLine(x0, yy0, x1, yy1, YELLOW);
      else if (i == 290 || i == 70 || i == 250 || i == 110 ) sc3.drawLine(x0, yy0, x1, yy1, RED);

      if (i % 5 == 0 && i % 10 != 0)  sc3.drawLine(x0, yy0, x2, yy2, 15);

      if (i == ang) sc3.drawLine(x0, yy0, x3, yy3, 15);


    }
  }

  //sc1.pushSprite(0, 0, TFT_TRANSPARENT);
  tft.setPivot(22, 165);     // Set pivot to middle of TFT screen
  sc3.pushRotated(0);
  sc3.deleteSprite();
}

void drawScaleSprites1(int x, int y, int angle) {

  sc4.fillSprite(TFT_BLACK);
  // img.pushSprite(posx-x, posy-y, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  // img.deleteSprite();

  sc4.setColorDepth(4);
  sc4.createSprite(44, 80);
  sc4.createPalette(palette);

  int ang = 0;
  ang = 270 + angle;
  /*
    sc1.pushRotated(&sc2,360,TFT_BLACK);
    sc2.pushSprite(240, 0, TFT_BLACK);
    //tft.setPivot(220, 40);     // Set pivot to middle of TFT screen
    //sc2.pushRotated(180);
    sc2.deleteSprite();
  */



  // Draw 12 lines
  for (int i = 0 ; i < 360; i += 1) {
    if (  ( i >= 70 && i <= 110 ) ||
          ( i <= 290 && i >= 250 ) ) {

      sx = cos((i - 90) * 0.0174532925);
      sy = sin((i - 90) * 0.0174532925);
      x0 = sx * 114 + x;
      yy0 = sy * 114 + y;

      x1 = sx * 90 + x;
      yy1 = sy * 90 + y;

      x2 = sx * 100 + x;
      yy2 = sy * 100 + y;

      x3 = sx * 80 + x;
      yy3 = sy * 80 + y;





      if (ang >= 270 && i >= 270 ) {
        if (i <= ang) {
          if (i < 280) sc4.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i < 290 ) sc4.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc4.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (ang <= 270 && i < 270 ) {
        if (i >= ang) {
          if (i > 260) sc4.drawLine(x0, yy0, x2, yy2, GREEN);
          else if (i > 250 ) sc4.drawLine(x0, yy0, x2, yy2, YELLOW);
          else  sc4.drawLine(x0, yy0, x2, yy2, RED);
        }
      }

      if (i == 270 || i == 90) sc4.drawLine(x0, yy0, x1, yy1, GREEN );
      else if (i == 280 || i == 80 || i == 100 || i == 260) sc4.drawLine(x0, yy0, x1, yy1, YELLOW);
      else if (i == 290 || i == 70 || i == 250 || i == 110 ) sc4.drawLine(x0, yy0, x1, yy1, RED);

      if (i % 5 == 0 && i % 10 != 0)  sc4.drawLine(x0, yy0, x2, yy2, 15);

      if (i == ang) sc4.drawLine(x0, yy0, x3, yy3, 15);


    }
  }

  //sc1.pushSprite(0, 0, TFT_TRANSPARENT);
  tft.setPivot(218, 165);     // Set pivot to middle of TFT screen
  sc4.pushRotated(180);
  sc4.deleteSprite();
}


void drawScale(int x, int y) {


  // Draw clock face
  // tft.fillCircle(x, y, 120, TFT_GREEN);
  // tft.fillCircle(x, y, 118, TFT_BLACK);

  // Draw 12 lines
  for (int i = 0; i < 360; i += 5) {
    if ( ( i >= 70 && i <= 110 ) ||
         ( i <= 290 && i >= 250 )) {

      sx = cos((i - 90) * 0.0174532925);
      sy = sin((i - 90) * 0.0174532925);
      x0 = sx * 114 + x;
      yy0 = sy * 114 + y;

      x1 = sx * 100 + x;
      yy1 = sy * 100 + y;

      x2 = sx * 110 + x;
      yy2 = sy * 110 + y;

      if (i == 270 || i == 90)  tft.drawLine(x0, yy0, x1, yy1, TFT_GREEN);
      else if (i == 280 || i == 80 || i == 100 || i == 260)  tft.drawLine(x0, yy0, x1, yy1, TFT_YELLOW);
      else if (i == 290 || i == 70 || i == 250 || i == 110 )  tft.drawLine(x0, yy0, x1, yy1, TFT_RED);

      if (i % 10 != 0)  tft.drawLine(x0, yy0, x2, yy2, TFT_WHITE);

    }
  }

  /*
    // Draw 60 dots
    for(int i = 0; i<360; i+= 6) {
      sx = cos((i-90)*0.0174532925);
      sy = sin((i-90)*0.0174532925);
      x0 = sx*108+x;
      yy0 = sy*100+y;
      // Draw minute markers
      tft.drawPixel(x0, yy0, TFT_WHITE);

      // Draw main quadrant dots
      if(i==0 || i==180) tft.fillCircle(x0, yy0, 2, TFT_WHITE);
      if(i==90 || i==270) tft.fillCircle(x0, yy0, 2, TFT_WHITE);
    }
  */

}

// #########################################################################
// Draw compass using the defined transparent colour (takes ~6ms)
// #########################################################################
void drawCompass(int x, int y, int posx, int posy, int angle)
{
  TSTART
  // img.setColorDepth(16);
  //img.createSprite(150, 150);

  // TFT_TRANSPARENT is a special colour with reversible 8/16 bit coding
  // this allows it to be used in both 8 and 16 bit colour sprites.
  img.fillSprite(TFT_WHITE);
  // img.pushSprite(posx-x, posy-y, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  // img.deleteSprite();

  img.setColorDepth(16);
  img.createSprite((x * 2), (y * 2));

#define NEEDLE_L 42/2  // Needle length is 84, we want radius which is 42
#define NEEDLE_W 12/2  // Needle width is 12, radius is then 6

  // Draw the old needle position in the screen background colour so
  // it gets erased on the TFT when the sprite is drawn
  //img.fillTriangle(lx1,ly1,lx3,ly3,lx4,ly4,BACKGROUND);
  //img.fillTriangle(lx2,ly2,lx3,ly3,lx4,ly4,BACKGROUND);

  // Set text coordinate datum to middle centre
  img.setTextDatum(MC_DATUM);
  img.setTextColor(TFT_WHITE);


  // img.drawString("N",x,y-42,2);
  // img.drawString("E",x+42,y,2);
  // img.drawString("S",x,y+42,2);
  // img.drawString("W",x-42,9,2);
  img.pushImage(0, 25, alertWidth, alertHeight, alert);

  if (abs(angle) < 20)
    img.drawCircle(x, y, 30, TFT_DARKGREY);
  else if ( abs(angle) < 30)
    img.drawCircle(x, y, 30, TFT_YELLOW);
  else
    img.drawCircle(x, y, 30, TFT_RED);
  int angl = 270;

  //getCoord(x - 45, y, &lx1, &ly1, NEEDLE_L, angl);
  //getCoord(x - 45, y, &lx2, &ly2, NEEDLE_L, angl + 180);
  //getCoord(x - 45, y, &lx3, &ly3, NEEDLE_W, angl + 90);
  //getCoord(x - 45, y, &lx4, &ly4, NEEDLE_W, angl - 90);

  tft.setPivot(posx, posy);     // Set pivot to middle of TFT screen

  //img.fillTriangle(lx1, ly1, lx3, ly3, lx4, ly4, TFT_WHITE);
  //img.fillTriangle(lx2,ly2,lx3,ly3,lx4,ly4,TFT_LIGHTGREY);

  img.fillCircle(x, y, 3, TFT_DARKGREY);
  img.fillCircle(x, y, 2, TFT_LIGHTGREY);
  img.drawString(String(angle), x, y + 5, 6);


  //img.pushSprite(posx, posy, TFT_TRANSPARENT);

  img.setPivot(x, y);      // Set pivot relative to top left corner of Sprite

  img.pushRotated(angle);

  // Delete sprite to free up the RAM
  img.deleteSprite();
  //  langle = angle;
  TPRINT
}

void drawCompass2(int x, int y, int posx, int posy, int angle)
{
  TSTART


  // img.setColorDepth(16);
  // img.createSprite(150, 150);

  // TFT_TRANSPARENT is a special colour with reversible 8/16 bit coding
  // this allows it to be used in both 8 and 16 bit colour sprites.
  //img.fillSprite(TFT_BLACK);
  //img.pushSprite(posx-x, posy-y, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  // img.deleteSprite();

  imgs.setColorDepth(16);
  imgs.createSprite((x * 2), (y * 2));

  // TFT_TRANSPARENT is a special colour with reversible 8/16 bit coding
  // this allows it to be used in both 8 and 16 bit colour sprites.
  imgs.fillSprite(TFT_BLACK);
  //img.pushSprite(posx-50, posy-50, TFT_BLACK);
  //img.setPivot(50, 50);      // Set pivot relative to top left corner of Sprite
  //img.deleteSprite();

  //img.setColorDepth(8);
  //img.createSprite(100, 100);

#define NEEDLE_L 84/2  // Needle length is 84, we want radius which is 42
#define NEEDLE_W 12/2  // Needle width is 12, radius is then 6

  // Draw the old needle position in the screen background colour so
  // it gets erased on the TFT when the sprite is drawn
  //img.fillTriangle(lx1,ly1,lx3,ly3,lx4,ly4,BACKGROUND);
  //img.fillTriangle(lx2,ly2,lx3,ly3,lx4,ly4,BACKGROUND);

  // Set text coordinate datum to middle centre
  imgs.setTextDatum(MC_DATUM);
  imgs.setTextColor(TFT_WHITE);

  //imgs.drawString("N",x,y-42,2);
  //imgs.drawString("E",x+42,y,2);
  //imgs.drawString("S",x,y+42,2);
  //imgs.drawString("W",x-42,9,2);
  imgs.pushImage(0, 25, backWidth, backHeight, back);
  imgs.drawCircle(x, y, 30, TFT_DARKGREY);
  int angl = 90;

  //getCoord(x, y, &lx1, &ly1, NEEDLE_L, angl);
  // getCoord(x, y, &lx2, &ly2, NEEDLE_L, angl+180);
  // getCoord(x, y, &lx3, &ly3, NEEDLE_W, angl+90);
  // getCoord(x, y, &lx4, &ly4, NEEDLE_W, angl-90);
  tft.setPivot(posx, posy);     // Set pivot to middle of TFT screen

  //img.fillTriangle(lx1,ly1,lx3,ly3,lx4,ly4,TFT_RED);
  //img.fillTriangle(lx2,ly2,lx3,ly3,lx4,ly4,TFT_LIGHTGREY);

  imgs.fillCircle(x, y, 3, TFT_DARKGREY);
  imgs.fillCircle(x, y, 2, TFT_LIGHTGREY);
  imgs.drawString(String(angle), x, y + 5, 6);


  //img.pushSprite(posx, posy, TFT_TRANSPARENT);

  imgs.setPivot(x, y);      // Set pivot relative to top left corner of Sprite

  imgs.pushRotated(angle);

  // Delete sprite to free up the RAM
  imgs.deleteSprite();
  //  langle = angle;
  TPRINT
}


void drawCompass22(int x, int y, int posx, int posy, int angle)
{
  TSTART
  imgs.setColorDepth(8);
  imgs.createSprite(100, 100);

  // TFT_TRANSPARENT is a special colour with reversible 8/16 bit coding
  // this allows it to be used in both 8 and 16 bit colour sprites.
  imgs.fillSprite(TFT_TRANSPARENT);

#define NEEDLE_L 42/2  // Needle length is 84, we want radius which is 42
#define NEEDLE_W 12/2  // Needle width is 12, radius is then 6

  // Draw the old needle position in the screen background colour so
  // it gets erased on the TFT when the sprite is drawn
  imgs.fillTriangle(lxs1, lys1, lxs3, lys3, lxs4, lys4, BACKGROUND);
  imgs.fillTriangle(lxs2, lys2, lxs3, lys3, lxs4, lys4, BACKGROUND);

  // Set text coordinate datum to middle centre
  imgs.setTextDatum(MC_DATUM);
  imgs.setTextColor(TFT_WHITE);

  //imgs.drawString("N",50,50-42,2);
  //imgs.drawString("E",50+42,50,2);
  //imgs.drawString("S",50,50+42,2);
  //imgs.drawString("W",50-42,50,2);

  imgs.drawCircle(x, y, 30, TFT_DARKGREY);

  //getCoord(x, y, &lxs1, &lys1, NEEDLE_L, angle);
  //getCoord(x, y, &lxs2, &lys2, NEEDLE_L, angle+180);
  //getCoord(x, y, &lxs3, &lys3, NEEDLE_W, angle+90);
  //getCoord(x, y, &lxs4, &lys4, NEEDLE_W, angle-90);

  //imgs.fillTriangle(lxs1,lys1,lxs3,lys3,lxs4,lys4,TFT_RED);
  //imgs.fillTriangle(lxs2,lys2,lxs3,lys3,lxs4,lys4,TFT_LIGHTGREY);

  imgs.fillCircle(x, y, 3, TFT_DARKGREY);
  imgs.fillCircle(x, y, 2, TFT_LIGHTGREY);

  imgs.pushSprite(posx - 50, posy - 50, TFT_TRANSPARENT);

  // Delete sprite to free up the RAM
  imgs.deleteSprite();
  TPRINT
}


#define RAD2DEG 0.0174532925

// Get coordinates of end of a vector, centre at x,y, length r, angle a
// Coordinates are returned to caller via the xp and yp pointers
void getCoord(int x, int y, int *xp, int *yp, int r, int a)
{
  float sx1 = cos( (a - 90) * RAD2DEG );
  float sy1 = sin( (a - 90) * RAD2DEG );
  *xp =  sx1 * r + x;
  *yp =  sy1 * r + y;
}
