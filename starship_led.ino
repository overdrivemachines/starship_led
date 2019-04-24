#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

#define NUM_LEDS 384 // 362. 363 to 384
#define BOARD_LEDS 361

#define BRIGHTNESS 100
//#define BOARD_BRIGHTNESS 50

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_RGB + NEO_KHZ800);

byte neopix_gamma[] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

// loop counters
int i, j = BOARD_LEDS;

// Random number
uint8_t r; // 0..255
uint32_t randomColor;

bool redToggle = true;

String inputString = "";      // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

int mode = 0; // Mode will be set by input from serial
bool is_red_alert = false; // is red alert mode activated?

void setup() {
  // initialize serial:
  Serial.begin(9600);
  // reserve 8 bytes for the inputString:
  inputString.reserve(8);
  
  strip.setBrightness(BRIGHTNESS);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  // start serial port at 9600 bps and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  // turn off all pixels
  for (i = 0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();

  delay(800);

  // turn all board pixels blue
  for (i = 0; i < NUM_LEDS; i++)
  {
    strip.setPixelColor(i, 0, 0, 255);
  }
  strip.show();
}

void loop() 
{
  if (stringComplete) 
  {
    Serial.println(inputString);
    if (inputString.toInt() == 6)
    {
      is_red_alert = true;
      Serial.println("Red Alert Activated");
    }
    else // should be 7 but any other number should work 
    {
      is_red_alert = false;
      Serial.println("Red Alert Deactivated");
    }

    // clear the string:
    inputString = "";
    stringComplete = false;    
  }

  if (is_red_alert)
  {
    redAlertLED();
    delay(600);
  }
  else
  {
    randomChange(20);
    delay(random(0, 800));
  }
}

void redAlertLED() 
{
  if (redToggle) 
  {
    for (i = 0; i < NUM_LEDS; i++)
      strip.setPixelColor(i, 255, 0, 0);
    redToggle = false;
  }
  else
  {
    for (i = 0; i < NUM_LEDS; i++)
      strip.setPixelColor(i, 20, 0, 0);
    redToggle = true;
  }
  strip.show();
}

// There is a percent% chance an LED would change color
void randomChange(uint8_t percent) 
{
  // iterate through all LEDs
  for (i = 0; i < BOARD_LEDS; i++) 
  {
    // generate random number between 0 and 99
    r = random(0, 100);
    if (r < percent)
    {
      // 10% of the time set to red or green
      if (random(0, 100) < 10)
      {
        // 50% of the time set to red
        if (random(0,100) < 50)
          randomColor = strip.Color(255, 0, 0);
        else
          randomColor = strip.Color(0, 255, 0);
      }
      else
      {
        // Generate random color
        randomColor = strip.Color(random(0,255), random(0,255), random(0,255));
      }
      
      strip.setPixelColor(i, randomColor);
    }
    else
    {
      // Change 50% of the time
      if (random(0, 100) < 50)
      {
        // Change 1% of the time to blue
        if (random(0, 100) < 1)
          strip.setPixelColor(i, 0, 0, 255);
        else
          strip.setPixelColor(i, 0, 0, 0);
      }      
    }
  }

  // circular LEDs
  if (j < NUM_LEDS)
  {
    // strip.setPixelColor(j, strip.Color(255, 255, 255));
    strip.setPixelColor(j, 255, 255, 255);
    if (j - BOARD_LEDS >= 3)
      strip.setPixelColor(j - 3, 0, 0, 255);
    else
    {
      strip.setPixelColor(NUM_LEDS - 3 + (j - BOARD_LEDS), 0, 0, 255);
    }
    j = j + 1;
  }
  else
  {
    j = BOARD_LEDS;
  }
  strip.show();
}

/*
  SerialEvent occurs whenever a new data comes in the hardware serial RX. This
  routine is run between each time loop() runs, so using delay inside loop can
  delay response. Multiple bytes of data may be available.
*/
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

