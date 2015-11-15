/******************************************************
//Some crazy colours and patterns using FastLED palettes
//and some random bits of code from the interwebs
//built to working with Marc Miller's arduino to processing v10
//Processing sketch (modified for POV display). Using Arduino 1.0.6 and Processing 2.2.1
//starts off slow wait for the good bits coming later
//this is just a testing sketch so don't expect any code to make sense
//tested working on Arduino Nano with ws2811 36 LED's 144px/m strip
*//////////////////////////////////////////////////////

#include <FastLED.h>

#define LED_PIN     6
#define NUM_LEDS    37
#define BRIGHTNESS  255 //change to 20 for testing on strip!!!!
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

boolean upDown = true;
boolean lines = true;

#define UPDATES_PER_SECOND 30000

// This example shows several ways to set up and use 'palettes' of colors
// with FastLED.
//
// These compact palettes provide an easy way to re-colorize your 
// animation on the fly, quickly, easily, and with low overhead.
//
// USING palettes is MUCH simpler in practice than in theory, so first just
// run this sketch, and watch the pretty lights as you then read through 
// the code.  Although this sketch has eight (or more) different color schemes,
// the entire sketch compiles down to about 6.5K on AVR.
//
// FastLED provides a few pre-configured color palettes, and makes it
// extremely easy to make up your own color schemes with palettes.
//
// Some notes on the more abstract 'theory and practice' of 
// FastLED compact palettes are at the bottom of this file.



CRGBPalette16 currentPalette;
TBlendType    currentBlending;

extern CRGBPalette16 myRedWhiteBluePalette;
extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

int paletteVar = 1;

int motionSpeed = 1;
int maxStartIndex = 70;
int minStartIndex = 0;
volatile int setting = 2;

/****Variables needed for sending to Processing. */
uint16_t sendDelay = 10;    // [Milliseconds] To slow stuff down if needed.
boolean testing = false;  // Default is false. [Ok to change for testing.]
  // Can temporarily change testing to true to check output in serial monitor.
  // Must set back to false to allow Processing to connect and receive data.

boolean linkedUp = false;  // Initially set linkup status false. [Do Not change.]
/****End of variables needed for sending Processing. */


void setup() {
  //delay( 3000 ); // power-up safety delay - always use for testing LED strip
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness(  BRIGHTNESS );
  
  //currentPalette = RainbowStripeColors_p;
  currentBlending = NOBLEND;
  /****Stuff needed in setup() section for sending to Processing. */
  Serial.begin(115200);  // Allows serial output.
  while (!Serial){ ; }  // Wait for serial connection. Only needed for Leonardo board.
  firstContact();  // Connect with Processing. Hello, is anyone out there?
  /****End of stuff to put in your setup() section for Processing. */

}


void loop()
{
  /****This tests if serial is connected.  Needed for sending to Processing. */
if (linkedUp == true) {  /****Check to see if connected with Processing. */

 if(setting == 1){
   
  if(lines == false){ //toggled in ChangePalettePeriodically3()
  //ChangePalettePeriodically2();
  //ChangePaletteEveryTime();
  //Serial.println(paletteVar);
  //PaletteSetup();
  //SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
  //SetupRandomVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
  ChangePalettePeriodically3();  
  static uint8_t startIndex = 0;
  if(upDown == true) {
  startIndex = startIndex + motionSpeed; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  //Serial.println(startIndex);
    if (startIndex == maxStartIndex){
    upDown = false;
    }
  }
  else {
    startIndex = startIndex - motionSpeed; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  //Serial.println(startIndex);
    if (startIndex == minStartIndex){
    upDown = true;
    }   
  }
  
   //add_glitter();
   
  FastLED.show(); SendToProcessing();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  /*
  motionSpeed++;
  if(motionSpeed == 10){
    motionSpeed = 1;
  }
  
  minStartIndex++;
  if(minStartIndex == 20){
    minStartIndex = 0;
  }
  maxStartIndex--;
  if(maxStartIndex == 30){
    maxStartIndex = 70;
  }
 */ 
  }//end if(lines)
  else{
    //ChangePalettePeriodically2();
  //ChangePaletteEveryTime();
  //Serial.println(paletteVar);
  //PaletteSetup();
  //SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
  //SetupRandomVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black);
  ChangePalettePeriodically3();  
  static uint8_t startIndex = 0;
  startIndex = startIndex + motionSpeed; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  //Serial.println(startIndex);
    if (startIndex == maxStartIndex){
    startIndex = 0;
    }
   //add_glitter();
   //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
  FastLED.show(); SendToProcessing();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  /*
  motionSpeed++;
  if(motionSpeed == 10){
    motionSpeed = 1;
  }
  
  minStartIndex++;
  if(minStartIndex == 20){
    minStartIndex = 0;
  }
  maxStartIndex--;
  if(maxStartIndex == 30){
    maxStartIndex = 70;
  }
 */ 
    
  }//end else(lines)
 }//end if(setting ==1) 
 
 else if(setting == 2)
 {
   ChangeStripesPeriodically();
   static uint8_t stripeIndex = 0;
  stripeIndex = stripeIndex + 1; 
  FillStripesFromPaletteColors( stripeIndex);
  //Serial.println(startIndex);
    if (stripeIndex > 15){
    stripeIndex = 0;
    }
   //add_glitter();
   //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
  FastLED.show(); SendToProcessing();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
 }//end if(setting == 2)
 else
 {
   ChangeStripesPeriodically();
   static uint8_t stripeIndex2 = 0;
  stripeIndex2 = stripeIndex2 + 1; 
  FillPatternStripesFromPaletteColors( stripeIndex2, 4);
  //Serial.println(startIndex);
    if (stripeIndex2 > 15){
    stripeIndex2 = 0;
    }
   //add_glitter();
   //add_glitter from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
  FastLED.show(); SendToProcessing();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
 }//end else 3
 
 //}//end switch(setting)
 }
}//end loop

 

void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex += 3; //this is something to change the pattern nicely!
  }
  
}

void FillStripesFromPaletteColors( uint8_t colorIndex)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex = colorIndex + colorIndex % 4; //change this to change pattern nicely!!
  }
  
}

void FillPatternStripesFromPaletteColors( uint8_t colorIndex, uint8_t var)
{
  uint8_t brightness = 255;
  
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, brightness, currentBlending);
    colorIndex = colorIndex + var; //change this to change pattern nicely!!
  }
  
}

// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

void PaletteSetup(){
  //currentPalette = RainbowColors_p;         currentBlending = BLEND; //not great needs modification for best pov look
  currentPalette = RainbowStripeColors_p;   currentBlending = NOBLEND;  //super awesome rainbows!
  //currentPalette = RainbowStripeColors_p;   currentBlending = BLEND; //also fantastic!
  //SetupPurpleAndGreenPalette();             currentBlending = BLEND; //good one
  //SetupTotallyRandomPalette();              currentBlending = BLEND; //too random
  //SetupBlackAndWhiteStripedPalette();       currentBlending = NOBLEND; //striking...
  //SetupBlackAndWhiteStripedPalette();       currentBlending = BLEND; //nice too...
  //currentPalette = CloudColors_p;           currentBlending = BLEND; //needs more contrast
  //currentPalette = PartyColors_p;           currentBlending = BLEND; //also needs contrast
  //currentPalette = myRedWhiteBluePalette_p; currentBlending = NOBLEND;  //yes yes yes it works
  //currentPalette = myRedWhiteBluePalette_p; currentBlending = BLEND; //nice
}

void ChangePalettePeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    //if( secondHand == 1)   { SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black); }
    if( secondHand == 15)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    //if( secondHand == 20)  { SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black); }
    if( secondHand == 25)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    //if( secondHand == 30)  { SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black); }
    if( secondHand == 35)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
   //if( secondHand == 40)  { SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black); }
    if( secondHand == 45)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    //if( secondHand == 50)  { SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black); }
    if( secondHand == 55)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
  }
}


void ChangePalettePeriodically2()
{
  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    //if( secondHand == 1)   { SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black); }
    if( secondHand == 15)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    //if( secondHand == 20)  { SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black); }
    if( secondHand == 25)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    //if( secondHand == 30)  { SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black); }
    if( secondHand == 35)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
   //if( secondHand == 40)  { SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black); }
    if( secondHand == 45)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    //if( secondHand == 50)  { SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black); }
    if( secondHand == 55)  { SetupRandomVariablePalette2(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
  }
}

void ChangePalettePeriodically3()
{
  uint8_t secondHand = (millis() / 1000) % 120; //last number is number of seconds the whole thing lasts!
  static uint8_t lastSecond = 121;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand == 1)   { SetupVariablePalette3(CRGB::Red, CRGB::Black, CRGB::Blue); }
    if( secondHand == 10)  { SetupVariablePalette3(CRGB::Magenta, CRGB::Black, CRGB::Green); }
    if( secondHand == 20)  { SetupVariablePalette3(CRGB::Cyan, CRGB::Black, CRGB::Yellow); }
    if( secondHand == 30)  { SetupVariablePalette3(CRGB::Red, CRGB::Black, CRGB::Green); }
    if( secondHand == 40)  { SetupVariablePalette3(CRGB::Cyan, CRGB::Black, CRGB::Magenta); }
    if( secondHand == 50)  { SetupVariablePalette3(CRGB::Black, CRGB::Red, CRGB::Cyan); }
    
    if( secondHand == 60)  { SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Blue, CRGB::Black); }
    if( secondHand == 70)  { SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Yellow, CRGB::Black); }
    if( secondHand == 80)  { SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Green, CRGB::Black); }
    if( secondHand == 90)  { SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Magenta, CRGB::Black); }
    if( secondHand == 100) { SetupVariablePalette(CRGB::Green, CRGB::Black, CRGB::Blue, CRGB::Black); }
    
    if( secondHand == 60)  { SetupVariablePalette5(CRGB::BlueViolet, CRGB::Black, CRGB::Amethyst, CRGB::Black, CRGB::Coral); }
    if( secondHand == 70)  { SetupVariablePalette5(CRGB::Chocolate, CRGB::Black, CRGB::DarkCyan, CRGB::Black, CRGB::DarkGoldenrod); }
    if( secondHand == 80)  { SetupVariablePalette5(CRGB::DarkOliveGreen, CRGB::DodgerBlue, CRGB::Green, CRGB::Black, CRGB::DarkOrchid); }
    if( secondHand == 90)  { SetupVariablePalette5(CRGB::Lavender, CRGB::Black, CRGB::IndianRed, CRGB::Black, CRGB::FloralWhite); }
    if( secondHand == 100) { SetupVariablePalette5(CRGB::MediumTurquoise, CRGB::Black, CRGB::Blue, CRGB::Black, CRGB::Lime); }
    if( secondHand == 110) { 
      //toggle between lines and zigzag pattern (using same palettes above!)
      if(lines == false){
        lines=true;
      }
      else{
        lines=false;
      }
      setting = 2;
  
    }
  
    

  }
}

void ChangeStripesPeriodically()
{
  uint8_t secondHand = (millis() / 1000) % 60; //last number is number of seconds the whole thing lasts!
  static uint8_t lastSecond = 99;
  
  if( lastSecond != secondHand) {
    lastSecond = secondHand;
    if( secondHand == 1 )  { SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Yellow, CRGB::Black); }
    if( secondHand == 10)  { SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Blue, CRGB::Black); }
    if( secondHand == 20)  { SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Yellow, CRGB::Black); }
    if( secondHand == 30)  { SetupVariablePalette(CRGB::Red, CRGB::Black, CRGB::Green, CRGB::Black); }
    if( secondHand == 40)  { SetupVariablePalette(CRGB::Cyan, CRGB::Black, CRGB::Magenta, CRGB::Black); }
    if( secondHand == 50)  { SetupVariablePalette(CRGB::Green, CRGB::Black, CRGB::Blue, CRGB::Black); }
    if( secondHand == 59)  { 
                              setting++;
                              if(setting > 3){setting = 1;} 
                           }

    
    }
}


//SetupRandomVariablePalette2(CRGB::Red, CRGB::Green, CRGB::Blue, CRGB::Magenta);

void ChangePaletteEveryTime()
{
    paletteVar++;
    //Serial.println(paletteVar);
    if( paletteVar == 1)   { SetupVariablePalette(CRGB::Red, CRGB::Gray, CRGB::Blue, CRGB::Black); }
    if( paletteVar == 2)  { SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    if( paletteVar == 3)  { SetupVariablePalette(CRGB::Green, CRGB::Gray, CRGB::Pink, CRGB::Black); }
    if( paletteVar == 4)  { SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    if( paletteVar == 5)  { SetupVariablePalette(CRGB::Orange, CRGB::Gray, CRGB::Cyan, CRGB::Black); }
    if( paletteVar == 6)  { SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
   if( paletteVar == 7)  { SetupVariablePalette(CRGB::Magenta, CRGB::Gray, CRGB::Yellow, CRGB::Black); }
    if( paletteVar == 8)  { SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    if( paletteVar == 9)  { SetupVariablePalette(CRGB::Blue, CRGB::Gray, CRGB::Green, CRGB::Black); }
    if( paletteVar == 10)  { SetupRandomVariablePalette(CRGB::Black, CRGB::Gray, CRGB::Black, CRGB::Gray); }
    if (paletteVar == 11) { paletteVar = 0; }
    
  
}

// This function fills the palette with totally random colors.
void SetupTotallyRandomPalette()
{
  for( int i = 0; i < 16; i++) {
    currentPalette[i] = CHSV( random8(), 255, random8());
  }
}

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.
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

void SetupVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d)//4 colours default
{
  
    currentPalette=CRGBPalette16(a, b, c, d, a, b, c, d, a, b, c, d, a, b, c, d);
}

void SetupVariableStripes(CRGB a, CRGB b, CRGB c, CRGB d)//4 colours default
{
  
    for(int i = 0; i < NUM_LEDS; i++){
      leds[i] = a;
    }
}

void SetupVariablePalette3(CRGB a, CRGB b, CRGB c) //3 colours
{
    currentPalette=CRGBPalette16(a, b, c, a, b, c, a, b, c, a, b, c, a, b, c, a);
}

void SetupVariablePalette5(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e) //5 colours
{
    currentPalette=CRGBPalette16(a, b, c, d, e, a, b, c, d, e, a, b, c, d, e, a);
}

void SetupVariablePalette6(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e, CRGB f) //6 colours
{
    currentPalette=CRGBPalette16(a, b, c, d, e, f, a, b, c, d, e, f, a, b, c, d);
}

void SetupVariablePalette7(CRGB a, CRGB b, CRGB c, CRGB d, CRGB e, CRGB f, CRGB g) //7 colours
{
    currentPalette=CRGBPalette16(a, b, c, d, e, f, g, a, b, c, d, e, f, g, a, b);
}



void SetupRandomVariablePalette(CRGB a, CRGB b, CRGB c, CRGB d)
{
    CRGB rand1 = CHSV( random8(), 255, 255);
    CRGB rand2 = CHSV( random8(), 255, 255);
    CRGB rand3 = rand1-rand2;
    //function to mix up order of a, b, c, d here? if 1 currentPalette is order no.1 if 2 currentPalette is order no.2
    currentPalette=CRGBPalette16(a, rand1, c, rand2, b, rand1, d, rand2, a, rand1, c, rand2, b, rand1, d, rand2);
}

void SetupRandomVariablePalette2(CRGB a, CRGB b, CRGB c, CRGB d)
{
// 'black out' all 16 palette entries...
  fill_solid( currentPalette, 16, CRGB::Black);
  // and set every fourth one to whatever.
  int r1 = random8(13);
  int r2 = random8(13);
  int r3 = random8(13);
  int r4 = random8(13);
  currentPalette[r1] = a;
  currentPalette[r2] = b;
  currentPalette[r3] = c;
  currentPalette[r4] = d;
}

void add_glitter() //from https://gist.github.com/kriegsman/ccffc81a74bc03636ce1
{
  int chance_of_glitter =  10; // percent of the time that we add glitter
  int number_of_glitters = 5; // number of glitter sparkles to add
  
  int r = random8(100);
  if( r < chance_of_glitter ) {
    for( int j = 0; j < number_of_glitters; j++) {
      int pos = random16( NUM_LEDS);
      leds[pos] = CRGB::White; // very bright glitter
    }
  }
}


// This example shows how to set up a static color palette
// which is stored in PROGMEM (flash), which is almost always more 
// plentiful than RAM.  A static PROGMEM palette like this
// takes up 64 bytes of flash.
const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM =
{
  CRGB::Red,
  CRGB::Gray, // 'white' is too bright compared to red and blue
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Black,

  CRGB::Red,
  CRGB::Red,
  CRGB::Gray,
  CRGB::Gray,
  CRGB::Blue,
  CRGB::Blue,
  CRGB::Black,
  CRGB::Black
};

/****The below two functions are needed for sending to Processing. */
// Copy from here to the bottom.
//--------------------
// Waits for Processing to respond and then sends the number of pixels.
void firstContact() {
  uint16_t nLEDS = NUM_LEDS;  // Number to send to Processing.  (Allows up to 65,535 pixels)
  if (testing == true){
    linkedUp = true;  // If testing, pretend we are already connected to Processing.
    Serial.print("NUM_LEDS: "); Serial.println(nLEDS);  // Number of pixels in our LED strip. 
    Serial.print("  High Byte = "); Serial.print(highByte(nLEDS));  // The high byte.
    Serial.print(" x 256 = "); Serial.println(highByte(nLEDS) * 256);
    Serial.print("  Low Byte  = "); Serial.println(lowByte(nLEDS));  // The low byte.
    delay(3000);
  }
  else {
    while (Serial.available() <= 0) {  // Repeats until Processing responds. Hello?
      Serial.print('A');  // send an ASCII A (byte of value 65)
      delay(100);
    }
    // Once Processing responds send the number of pixels as two bytes.
    Serial.write(highByte(nLEDS));  // Send the high byte to Processing.
    Serial.write(lowByte(nLEDS));  // Send the low byte to Processing.
    Serial.print('#');  // send an ASCII # (byte of value 35) as a flag for Processing.
    linkedUp = true;  // Now that Processing knows number of pixels set linkedUp to true.
    delay(500);
  }
}

//--------------------
// This function sends ALL the pixel data to Processing.
void SendToProcessing() {
  if (testing == true){  // Print pixel data. If NUM_LEDS is large this will be a lot of data!
    Serial.println("-------------------------------------------------------");
    for (uint16_t d=0; d < NUM_LEDS; d++){
      Serial.print("p: "); Serial.print(d);
      Serial.print("\tr: "); Serial.print(leds[d].r);
      Serial.print("\tg: "); Serial.print(leds[d].g);
      Serial.print("\tb: "); Serial.println(leds[d].b);
    }
    Serial.println(" ");
    delay(500);  // Add some extra delay while testing.
  }
  else {  // Send ALL the pixel data to Processing!
    for (uint16_t d=0; d < NUM_LEDS; d++){
      Serial.write(d);          // Pixel number
      Serial.write(leds[d].r);  // Red channel data
      Serial.write(leds[d].g);  // Green channel data
      Serial.write(leds[d].b);  // Blue channel data
    }
    delay(sendDelay);  // Delay to slow things down if needed.
  }
}

//--------------------
/****End of the functions needed for sending to Processing.*/


// Additionl notes on FastLED compact palettes:
//
// Normally, in computer graphics, the palette (or "color lookup table")
// has 256 entries, each containing a specific 24-bit RGB color.  You can then
// index into the color palette using a simple 8-bit (one byte) value.
// A 256-entry color palette takes up 768 bytes of RAM, which on Arduino
// is quite possibly "too many" bytes. 
//
// FastLED does offer traditional 256-element palettes, for setups that
// can afford the 768-byte cost in RAM.
//
// However, FastLED also offers a compact alternative.  FastLED offers
// palettes that store 16 distinct entries, but can be accessed AS IF
// they actually have 256 entries; this is accomplished by interpolating
// between the 16 explicit entries to create fifteen intermediate palette 
// entries between each pair.
//
// So for example, if you set the first two explicit entries of a compact 
// palette to Green (0,255,0) and Blue (0,0,255), and then retrieved 
// the first sixteen entries from the virtual palette (of 256), you'd get
// Green, followed by a smooth gradient from green-to-blue, and then Blue.

