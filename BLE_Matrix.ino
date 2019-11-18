#include <SoftwareSerial.h>
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Fonts/FreeMonoBoldOblique12pt7b.h>
#include <Fonts/Picopixel.h>
#include <FastLED.h>
#define NUM_LEDS_PER_STRIP 629 // or however many LEDs you have
#define NUM_STRIPS 1
#define DATA_PIN 7 // Or whatever pin you want that is included in those 3 sets
CRGB leds[NUM_LEDS_PER_STRIP * NUM_STRIPS];
#define LED_TYPE    WS2812B




SoftwareSerial mySerial(16, 17); // RX, TX
#define PIN 7
#define BRIGHTNESS 255

// Define matrix width and height.
#define mw 23
#define mh 23


Adafruit_NeoMatrix *matrix = new Adafruit_NeoMatrix(mw, mh, PIN,
    NEO_MATRIX_BOTTOM     + NEO_MATRIX_LEFT +
    NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
    NEO_GRB            + NEO_KHZ800);

// This could also be defined as matrix->color(255,0,0) but those defines
// are meant to work for adafruit_gfx backends that are lacking color()
#define LED_BLACK    0

#define LED_RED_VERYLOW   (3 <<  11)
#define LED_RED_LOW     (7 <<  11)
#define LED_RED_MEDIUM    (15 << 11)
#define LED_RED_HIGH    (31 << 11)

#define LED_GREEN_VERYLOW (1 <<  5)
#define LED_GREEN_LOW     (15 << 5)
#define LED_GREEN_MEDIUM  (31 << 5)
#define LED_GREEN_HIGH    (63 << 5)

#define LED_BLUE_VERYLOW  3
#define LED_BLUE_LOW    7
#define LED_BLUE_MEDIUM   15
#define LED_BLUE_HIGH     31

#define LED_ORANGE_VERYLOW  (LED_RED_VERYLOW + LED_GREEN_VERYLOW)
#define LED_ORANGE_LOW    (LED_RED_LOW     + LED_GREEN_LOW)
#define LED_ORANGE_MEDIUM (LED_RED_MEDIUM  + LED_GREEN_MEDIUM)
#define LED_ORANGE_HIGH   (LED_RED_HIGH    + LED_GREEN_HIGH)

#define LED_PURPLE_VERYLOW  (LED_RED_VERYLOW + LED_BLUE_VERYLOW)
#define LED_PURPLE_LOW    (LED_RED_LOW     + LED_BLUE_LOW)
#define LED_PURPLE_MEDIUM (LED_RED_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_PURPLE_HIGH   (LED_RED_HIGH    + LED_BLUE_HIGH)

#define LED_CYAN_VERYLOW  (LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_CYAN_LOW    (LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_CYAN_MEDIUM   (LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_CYAN_HIGH   (LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define LED_WHITE_VERYLOW (LED_RED_VERYLOW + LED_GREEN_VERYLOW + LED_BLUE_VERYLOW)
#define LED_WHITE_LOW   (LED_RED_LOW     + LED_GREEN_LOW     + LED_BLUE_LOW)
#define LED_WHITE_MEDIUM  (LED_RED_MEDIUM  + LED_GREEN_MEDIUM  + LED_BLUE_MEDIUM)
#define LED_WHITE_HIGH    (LED_RED_HIGH    + LED_GREEN_HIGH    + LED_BLUE_HIGH)

#define NUM_LEDS (mw * mh)
#define MAX_DIMENSION ((mw>mh) ?mw : mh)

// The leds
//CRGB leds[mw * mh];

// The 16 bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;
long previousMillis = 0;        // will store last time LED was updated

// the follow variables is a long because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long interval = 5000;           // interval at which to blink (milliseconds)
bool timebit=0;


const bool    kMatrixSerpentineLayout = true;

String bledata;






// We're using the x/y dimensions to map to the x/y pixels on the matrix.  We'll
// use the z-axis for "time".  speed determines how fast time moves forward.  Try
// 1 for a very slow moving effect, or 60 for something that ends up looking like
// water.
uint16_t speed = 10; // speed is set dynamically once we've started up

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise iwll be.  A value
// of 1 will be so zoomed in, you'll mostly see solid colors.
uint16_t scale = 30; // scale is set dynamically once we've started up

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

CRGBPalette16 currentPalette( PartyColors_p );
uint8_t       colorLoop = 1;

void setup()
{
  // Open serial communications and wait for port to open:
  Serial.begin(57600);

  //  while (!Serial) {
  //   ; // wait for serial port to connect. Needed for Leonardo only
  //  }


  //  Serial.println("Goodnight moon!");
 FastLED.addLeds<NUM_STRIPS, WS2812B, DATA_PIN, GRB>(leds, NUM_LEDS_PER_STRIP);
 // LEDS.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds,NUM_LEDS);
  LEDS.setBrightness(BRIGHTNESS);
  // set the data rate for the SoftwareSerial port
  matrix->begin();
  matrix->setFont(&Picopixel);
  matrix->setTextWrap(false);
  matrix->setBrightness(BRIGHTNESS);
  // Test full bright of all LEDs. If brightness is too high
  // for your current limit (i.e. USB), decrease it.
  // matrix->fillScreen(LED_WHITE_HIGH);
  matrix->setTextSize(1);
 
  matrix->setCursor(0, 4);
  matrix->setTextColor(LED_BLUE_HIGH);
  matrix->println("Zzz");
   matrix->setTextColor(LED_ORANGE_HIGH);
  matrix->print("  (-_-)");
  matrix->show();
  delay(500);
  matrix->setCursor(0, 4);
    matrix->setTextColor(LED_BLUE_HIGH);

  matrix->println("Zzz.");
     matrix->setTextColor(LED_ORANGE_HIGH);

  matrix->print("  (-_-)");
  matrix->show();
  delay(500);
  matrix->setCursor(0, 4);
    matrix->setTextColor(LED_BLUE_HIGH);

  matrix->println("Zzz..");
     matrix->setTextColor(LED_ORANGE_HIGH);

  matrix->print("  (-_-)");
  matrix->show();
  delay(500);
  matrix->setCursor(0, 4);
    matrix->setTextColor(LED_BLUE_HIGH);

  matrix->println("Zzz...");
     matrix->setTextColor(LED_ORANGE_HIGH);

  matrix->print("  (-_-)");
  matrix->show();
  delay(500);
  matrix->clear();
  matrix->show();
  matrix->setCursor(0, 4);
  matrix->println("");
  matrix->print("  (O_O)");
  matrix->show();
  delay(1000);
  /*
   matrix->clear();
  matrix->show();
  matrix->setCursor(0, 4);
  matrix->println("");
  matrix->print("  (^_^)");
  matrix->show();
  delay(1500);
  */
  matrix->clear();
  matrix->show();
  mySerial.begin(9600);
  // mySerial.println("Hello, world?");
}

void loop() // run over and over
{
  if (mySerial.available()) {
    bledata=mySerial.read();
    Serial.println(bledata);
   // 
  //  matrix->clear();
  //  matrix->setTextWrap(false);  // we don't wrap text so it scrolls nicely
   // matrix->setTextSize(1);
  //  matrix->setRotation(0);
  //  matrix->setTextColor(LED_ORANGE_HIGH);
  //  matrix->print(Serial.read());
  //  matrix->show();



  
  }
  if(bledata=="S"){
    
currentPalette = CloudColors_p;           speed =  4; scale = 30; colorLoop = 0;
  fillnoise8();
  
  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette();
  LEDS.show();
Serial.print("here");
  }
   if(bledata=="L"){
    
currentPalette = LavaColors_p;            speed =  8; scale = 50; colorLoop = 0;
  fillnoise8();
  
  // convert the noise data to colors in the LED array
  // using the current palette
  mapNoiseToLEDsUsingPalette();
  LEDS.show();
Serial.print("here");
  }
   if(bledata=="C"){
 matrix->clear();
  matrix->show();
Serial.print("here");
  }
//  if (Serial.available()) {
   // mySerial.write(Serial.read());

 // }










}















void fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }
  
  for(int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for(int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;
      
      uint8_t data = inoise8(x + ioffset,y + joffset,z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data,16);
      data = qadd8(data,scale8(data,39));

      if( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }
      
      noise[i][j] = data;
    }
  }
  
  z += speed;
  
  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;
}

void mapNoiseToLEDsUsingPalette()
{
  static uint8_t ihue=0;
  
  for(int i = 0; i <mw; i++) {
    for(int j = 0; j < mh; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if( colorLoop) { 
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the 
      // light/dark dynamic range desired
      if( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds[XY(i,j)] = color;
    }
  }
  
  ihue+=1;
}
void SetupBlackAndWhiteStripedPalette()
{
  // 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to white.
  currentPalette[0] = CRGB::White;
  currentPalette[4] = CRGB::White;
  currentPalette[8] = CRGB::White;
  currentPalette[12] = CRGB::White;

}

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;
  
  currentPalette = CRGBPalette16( 
    green,  green,  black,  black,
    purple, purple, black,  black,
    green,  green,  black,  black,
    purple, purple, black,  black );
}


//
// Mark's xy coordinate mapping code.  See the XYMatrix for more information on it.
//

uint16_t XY( uint8_t x, uint8_t y)
{
  uint16_t i;
  if( kMatrixSerpentineLayout == false) {
    i = (y *mw) + x;
  }
  if( kMatrixSerpentineLayout == true) {
    if( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (mw - 1) - x;
      i = (y *mw) + reverseX;
    } else {
      // Even rows run forwards
      i = (y *mw) + x;
    }
  }
  return i;
}
