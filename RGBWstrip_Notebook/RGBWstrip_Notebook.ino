// NeoPixel test program showing use of the WHITE channel for RGBW
// pixels only (won't look correct on regular RGB NeoPixel strips).

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN     D6

// How many NeoPixels are attached to the Arduino?
#define LED_COUNT  60

// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 255 // Set BRIGHTNESS to about 1/5 (max = 255)

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRBW + NEO_KHZ800);

void setup() {

  Serial.begin(115200);
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS);
}

void loop() {
 
  //colorRunForwards(strip.Color(0, 0, 0, 255),0 ,60, 20, 1);
  //colorRunBackwards(strip.Color(0, 0, 0, 255),60 ,0, 20, 1);
  //colorRunOverColor(strip.Color(0, 0, 0, 255), strip.Color(0, 255, 0, 0), 5, 20, 1);
  //fade(strip.Color(  0,   0,   0, 255), 5, 5);
  
  randomFade(5, 2, 1);
  delay(3000);
  randomFade(5, 2, 2);
  delay(3000);
  randomFade(5, 2, 3);
  delay(3000);
  //deskLight(strip.Color(  0,   0,   0, 255));

}
//////////////////////////////// Working //////////////////////////////////////////////////
void fade(uint32_t color, int wait, int cycles) {
  for(int i = 0; i < cycles; i++){
  strip.setBrightness(0);
    for(int j=0; j<256; j++) { // Ramp up from 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(color);
      strip.show();
      strip.setBrightness(j);
      delay(wait);
    }
  
    for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
      strip.fill(color);
      strip.show();
      strip.setBrightness(j);
      delay(wait);
    }
  }
}
///////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Working //////////////////////////////////////////////////
void randomFade(int wait, int cycles, int colorSpectrum) {
  for(int i = 0; i < cycles; i++){
    
  ////Replaced with randomColor function////////  
  //int r = random(0,256);
  //int g = random(0,256);
  //int b = random(0,256);
  //int w = random(0,128);
  //uint32_t color = strip.Color(r, g, b, 0);
  ///////////////////////////////////////////////
  
  uint32_t color = randomColorSpectrum(colorSpectrum);
  strip.setBrightness(0);
    for(int j=0; j<256; j++) { // Ramp up from 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(color);
      strip.show();
      strip.setBrightness(j);
      delay(wait);
    }

    for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
      strip.fill(color);
      strip.show();
      strip.setBrightness(j);
      delay(wait);
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////// Working //////////////////////////////////////////////////
void deskLight(int color) {
  for(int i=0; i<30; i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
  }
}
//////////////////////////////////////////////////////////////////////////////////////////

void whiteOverRainbow(int whiteSpeed, int whiteLength) {

  if(whiteLength >= strip.numPixels()) whiteLength = strip.numPixels() - 1;

  int      head          = whiteLength - 1;
  int      tail          = 0;
  int      loops         = 3;
  int      loopNum       = 0;
  uint32_t lastTime      = millis();
  uint32_t firstPixelHue = 0;

  for(;;) { // Repeat forever (or until a 'break' or 'return')
    for(int i=0; i<strip.numPixels(); i++) {  // For each pixel in strip...
      if(((i >= tail) && (i <= head)) ||      //  If between head & tail...
         ((tail > head) && ((i >= tail) || (i <= head)))) {
        strip.setPixelColor(i, strip.Color(0, 0, 255, 0)); // Set white
      } else {                                             // else set rainbow
        int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
        strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
      }
    }

    strip.show(); // Update strip with new contents
    // There's no delay here, it just runs full-tilt until the timer and
    // counter combination below runs out.

    firstPixelHue += 40; // Advance just a little along the color wheel

    if((millis() - lastTime) > whiteSpeed) { // Time to update head/tail?
      if(++head >= strip.numPixels()) {      // Advance head, wrap around
        head = 0;
        if(++loopNum >= loops) return;
      }
      if(++tail >= strip.numPixels()) {      // Advance tail, wrap around
        tail = 0;
      }
      lastTime = millis();                   // Save time of last movement
    }
  }
}

void pulseWhite(uint8_t wait) {
  for(int j=0; j<256; j++) { // Ramp up from 0 to 255
    // Fill entire strip with white at gamma-corrected brightness level 'j':
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }

  for(int j=255; j>=0; j--) { // Ramp down from 255 to 0
    strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
    strip.show();
    delay(wait);
  }
}

void rainbowFade2White(int wait, int rainbowLoops, int whiteLoops) {
  int fadeVal=0, fadeMax=100;

  // Hue of first pixel runs 'rainbowLoops' complete loops through the color
  // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
  // just count from 0 to rainbowLoops*65536, using steps of 256 so we
  // advance around the wheel at a decent clip.
  for(uint32_t firstPixelHue = 0; firstPixelHue < rainbowLoops*65536;
    firstPixelHue += 256) {

    for(int i=30; i<strip.numPixels(); i++) { // For each pixel in strip...

      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      uint32_t pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());

      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue, 255,
        255 * fadeVal / fadeMax)));
    }

    strip.show();
    

    if(firstPixelHue < 65536) {                              // First loop,
      if(fadeVal < fadeMax) fadeVal++;                       // fade in
    } else if(firstPixelHue >= ((rainbowLoops-1) * 65536)) { // Last loop,
      if(fadeVal > 0) fadeVal--;                             // fade out
    } else {
      fadeVal = fadeMax; // Interim loop, make sure fade is at max
    }
  }

  for(int k=0; k<whiteLoops; k++) {
    for(int j=0; j<256; j++) { // Ramp up 0 to 255
      // Fill entire strip with white at gamma-corrected brightness level 'j':
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
    
    for(int j=255; j>=0; j--) { // Ramp down 255 to 0
      strip.fill(strip.Color(0, 0, 0, strip.gamma8(j)));
      strip.show();
    }
  }

}
// Fill strip pixels one after another with a color. Strip is NOT cleared
// first; anything there will be covered pixel by pixel. Pass in color
// (as a single 'packed' 32-bit value, which you can get by calling
// strip.Color(red, green, blue) as shown in the loop() function above),
// and a delay time (in milliseconds) between pixels.
//////////////////////////////////////////////////////////////////////////////////////////



//////////////////////////////// Working //////////////////////////////////////////////////
void colorRunForwards(uint32_t color, int pixelStart, int pixelEnd, int wait, int cycles) {
  strip.setBrightness(BRIGHTNESS);
  for(int j = 0; j < cycles; j++){
    for(int i= pixelStart; i<= pixelEnd; i++) { // For each pixel in strip...
      strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
      strip.show();                          //  Update strip to match
      strip.setPixelColor(i-1, strip.Color(0, 0, 0, 0));
      strip.show();
      delay(wait);                           //  Pause for a moment
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Working //////////////////////////////////////////////////
void colorRunBackwards(uint32_t color, int pixelStart, int pixelEnd, int wait, int cycles) {
  strip.setBrightness(BRIGHTNESS);
  for(int j = 0; j < cycles; j++){
    for(int i= pixelStart; i>= pixelEnd; i--) { // For each pixel in strip...
      strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
      strip.show();                          //  Update strip to match
      strip.setPixelColor(i+1, strip.Color(0, 0, 0, 0));
      strip.show();
      delay(wait);                           //  Pause for a moment
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Working //////////////////////////////////////////////////
void colorRunOverColor(uint32_t color, uint32_t bgColor, int runSize ,int wait, int cycles) {
  strip.setBrightness(BRIGHTNESS);
  strip.fill(bgColor, 0, 60);
  for(int j = 0; j < cycles; j++){
    for(int i= 0; i<= LED_COUNT+runSize; i++) { // For each pixel in strip...
      strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
      strip.show();                          //  Update strip to match
      strip.setPixelColor(i-runSize, bgColor);
      strip.show();
      delay(wait);                           //  Pause for a moment
    }
  }
}
//////////////////////////////////////////////////////////////////////////////////////////


//////////////////////////////// Working //////////////////////////////////////////////////
uint32_t randomColor(bool white) {
  int r = random(200,256);
  int g = random(0,256);
  int b = random(0,256);
  if(white==true){
    int w = random(0,128);
    uint32_t color = strip.Color(r, g, b, w);
    return color;
  }if(white==false){ 
    uint32_t color = strip.Color(r, g, b, 0);
    return color;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////////// Working //////////////////////////////////////////////////
uint32_t randomColorSpectrum(int spectrum) {
  if(spectrum == 1){ //red spectrum
    int r = random(50,256);
    int g = random(0,100);
    int b = random(0,100);
    uint32_t color = strip.Color(r, g, b, 0);
    return color;
  }else if(spectrum == 2){ //green spectrum
    int r = random(0,100);
    int g = random(50,256);
    int b = random(0,100);
    uint32_t color = strip.Color(r, g, b, 0);
    return color;
  }else if(spectrum == 3){ //blue spectrum
    int r = random(0,100);
    int g = random(0,100);
    int b = random(50,256);
    uint32_t color = strip.Color(r, g, b, 0);
    return color;
  }
}
//////////////////////////////////////////////////////////////////////////////////////////
