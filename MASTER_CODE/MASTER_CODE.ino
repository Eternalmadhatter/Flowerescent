// Include the required Wire library for I2C<br>
#include<Wire.h>

#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014

#define DATA_PIN    13
//#define CLK_PIN   4
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define NUM_LEDS    19
CRGB leds[NUM_LEDS];
CRGB randomColor[20];
#define BRIGHTNESS         255
#define FRAMES_PER_SECOND  120
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

int countUp = 0;
long randNumber;

const int echoPin = 9;
const int trigPin = 8;
int distance = 0;
int x = 0;

int PressurePin0=A0;
int PressurePin1=A1;
int PressurePin2=A2;
int force0;
int force1;
int force2;

int trig[3] = {0, 0 ,0};

int trigPrv[3] = {0, 0 ,0};
void setup() {
   delay(3000); // 3 second delay for recovery
   pinMode(echoPin, INPUT);  
   pinMode(trigPin, OUTPUT);
   Serial.begin(9600); 

   FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  //FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);

  // set master brightness control
  FastLED.setBrightness(BRIGHTNESS);
  randomSeed (analogRead(0));
  // Start the I2C Bus as Master
  Wire.begin(); 

  Wire.beginTransmission(9); // transmit to device #9
  Wire.write(trig[0]);              // sends 0  
  Wire.write(trig[1]);              // sends 1  
  Wire.write(trig[2]);              // sends 2  
  Wire.endTransmission(); 
  delay(500);
}

/*// List of patterns to cycle through.  Each is defined as a separate function below.
typedef void (*SimplePatternList[])();
   SimplePatternList gPatterns = { rainbow, rainbowWithGlitter, confetti, sinelon, juggle, bpm };

uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns*/

void loop() {

  //float totaldist , avgDist;
  //totaldist=0;

  force0=analogRead(PressurePin0);                    //pressure sensor 1
  Serial.println(force0);
  force1=analogRead(PressurePin1);                    //pressure sensor 2
  Serial.println(force1);
  force2=analogRead(PressurePin2);                    //pressure sensor 3
  Serial.println(force2);
  /*for( int r=0; r<5; r++)
  {
     sendPing();
     int duration = pulseIn(echoPin, HIGH);
     distance = 0.034 * duration / 2;
     distance = clamp(distance, 0, 200);
     totaldist = totaldist+distance;
     
  }
   avgDist = totaldist/5;
   printToScreen(avgDist);*/
  if(force0<499 && force1<499 && force2<499) 
  {
   
   for(int i = 0; i<3;i++)
   {
      trig[i] = 0; 
   }
   randomColors();
   
  }
  else
  {
    if(force0>500)//If person step on rock 1
    {     
      trig[0]=1;
    }
    else
    {
      trig[0]=0;
    }
    if(force1>500)//If person step on rock 2
    {        
      trig[0]=1;
    }
    else
    {
      trig[1]=0;
    }
    if(force2>500)//if person step on rock 3
    {       
      trig[2]=1;  
    }
    else
    {
      trig[2]=0;
    }
  }
  if(trig[0] == trigPrv[0] && trig[1] == trigPrv[1] && trig[2] == trigPrv[2])
  {
    for( int i = 0 ; i<3 ; i++)
    {
      trigPrv[i] = trig[i];
    }

  }
  else
  {
    for( int i = 0 ; i<3 ; i++)
    {
      trigPrv[i] = trig[i];
    }

    if(trig[2] == 1)
    {
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(trig[2]);              // sends 0  
        Wire.endTransmission(); 
        OrRedFlowers();
    }
    else if(trig[1] == 1)
    {
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(trig[1]);              // sends 0  
        Wire.endTransmission(); 
        YellowFlowers();
    }
    else if(trig[0] == 1)
    {
        Wire.beginTransmission(9); // transmit to device #9
        Wire.write(trig[0]);              // sends 0  
        Wire.endTransmission(); 
        BuPuFlowers();
    }
    
  }
  /*Wire.beginTransmission(9); // transmit to device #9
  Wire.write(trig[0]);              // sends 0  
  Wire.write(trig[1]);              // sends 1  
  Wire.write(trig[2]);              // sends 2  
  Wire.endTransmission(); 
  delay(500);  */

  
 
   
  // insert a delay to keep the framerate modest
  FastLED.delay(1000/FRAMES_PER_SECOND); 

 /* // do some periodic updates
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
  // EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically*/
}

void randomColors()
{
  
  for (int i = 0; i <= 20; i++)//generates a list of 20 random numbers 
  {
    randNumber = random(0,40);
    randomColor[i] = randNumber;
  }

  for (int j = 0; j <= 20; j++)//marks the leds of those numbers as on 
  {
    if (j >= 0 && j < 6)
    {
      leds[randomColor[j]] = CRGB(222,10 ,10); //Rossa
      FastLED.show();
    } 
    else if (j >= 6 && j < 12)
    {
      leds[randomColor[j]] = CRGB(243, 91, 4); //Tangerine
      FastLED.show();
    }
    else if (j >= 12 && j < 20)
    {
      leds[randomColor[j]] = CRGB(10, 10, 255); // Blue
      FastLED.show();
    }
  }
  delay(300);
  for (int j = 0; j <= 20; j++)//clears LEDs for the nest set
  {
    leds[randomColor[j]] = CRGB(0,0 ,0); //Rossa
  }
  FastLED.show();
}

void BuPuFlowers()
{
  FastLED.clear();
  FastLED.show();
  for(countUp==0;countUp<=NUM_LEDS; countUp++)
  {
    if (countUp%2 == 0)
    {
      leds[countUp] = CRGB(10, 10, 255); //blue
      
    }
    if (countUp%2 == 1)
    {
      leds[countUp] = CRGB(50, 10, 200); //purple;
    }
    FastLED.show();
  }
}

void YellowFlowers()
{
  FastLED.clear();
  FastLED.show();
  for(countUp==0;countUp<=NUM_LEDS; countUp++)
  {
    if (countUp%2 == 0){
    leds[countUp] = CRGB(247, 184, 1); //selective yellow
    }
    
    if (countUp%2 == 1){
    leds[countUp] = CRGB(243, 91, 4); //tangerine
    }
    FastLED.show();
    delay(50);
  }
}

void OrRedFlowers()
{
  FastLED.clear();
  FastLED.show();
  for(countUp==0;countUp<=NUM_LEDS; countUp++)
  { 
    if (countUp%2 == 0)
    {
      leds[countUp] = CRGB(220, 47, 2); //persimmon
    }
    
    if (countUp%2 == 1)
    {
      leds[countUp] = CRGB(222,10 ,10); //Rosso corsa
    }
    FastLED.show();
    delay(10);
  }
}



/*void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
   gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) 
  {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS-1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for( int i = 0; i < NUM_LEDS; i++) 
  { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for( int i = 0; i < 8; i++) 
  {
    leds[beatsin16( i+7, 0, NUM_LEDS-1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}*/

/*void sendPing()
{
   digitalWrite(trigPin, LOW);
   delayMicroseconds(5);
   digitalWrite(trigPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(trigPin, LOW);
}
int clamp(int VAL, int min, int max)
{
  if(VAL <= min)
  {
    VAL = min;
    return VAL;
  }
  else if(VAL >= max)
  {
    VAL = max;
    return VAL;
  }
  else
  {
    return VAL;
  }
}

void printToScreen(int TARGET)
{
    Serial.print(TARGET);
    Serial.println(" cm");
}*/
