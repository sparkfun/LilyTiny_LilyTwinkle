/******************************************************************************
  LilyTiny (ATtiny85, internal 8 MHz clock)
  Emily Lovell
 
  Controls the behavior of up to four LEDs, depending on pin:
    Pin 0: fades in and out
    Pin 1: heartbeat
    Pin 2: blinks on and off
    Pin 3: twinkles randomly
    
 AVRDUDE fuse settings string:
 avrdude -p t85 -c avrispmkii -B 4 -P usb -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
 By the time most users see this board, these settings will already 
  have been applied. However, when programming from a blank ATTiny, it
  is necessary to program the fuse bits to disable the divide-by-8
  on the clock frequency.
******************************************************************************/

// Constant definitions
#define maxBrightness 255.0

#define maxTimeOn 9999 
#define blinkTime 125 

// Variables and constants for fading pin. Needs to be a pin with PWM hardware
//  available; on the Tiny85 that's pin 0 and pin 1 only.
#define fadePin 	0  			// Redefine the pin name to something
								//  application specific.
int fadeDirection 	= 1; 		// 1 is brighter, -1 is dimmer
int fadeStep 		= 1; 
int fadeBrightness 	= 1; 

// Variables and constants for fading pin. Needs to be a hardware PWM pin; on
//  Tiny85 that's pin 0 and pin 1 only.
#define HEARTPIN	1 			// Redefine the pin name to something
								//  application specific.
int heartStage = 0;
float heartBrightness = 0;
float heartCounter = 0;

// variables for blink pin (pin 2)
int blinkPin = 2; 
int blinkDelay = blinkTime;
int blinkState = false;

// variables for twinkle pin (pin 3)
int twinklePin = 3; 
int timeOn = 0;
float currentBrightness = 0;
int targetBrightness = 0;
float timeToBright = 0;
float stepToBright = 0;


void setup()
{
  pinMode(twinklePin, OUTPUT);
  startOver();
  pinMode(blinkPin, OUTPUT);
}

void loop()
{
  // calculate brightness for twinkle pin (determined 
  // by how much time the LED is on vs. off) 
  timeOn = calcTimeOn(currentBrightness);
  digitalWrite(twinklePin, HIGH);
  delayMicroseconds(timeOn + 1);
  digitalWrite(twinklePin, LOW);
  delayMicroseconds(maxTimeOn - timeOn);

  // recalculate brightness for twinkle pin
  currentBrightness += stepToBright;
  timeToBright--;
  if(timeToBright == 0)
    startOver();
 
  // calculate brightness for heartbeat pin (by cycling 
  // through phases of pulse pattern)
  heartCounter++;
  if (heartCounter > 144) {
    heartStage = (heartStage + 1) % 4;
    heartCounter = 0;
  }
  else {
    switch(heartStage) {
      case 0:
        analogWrite(HEARTPIN, heartBrightness);
        heartCounter += 144/12.0;
        heartBrightness += 255/12.0;
        if (heartBrightness > 255)
          heartBrightness = 255;
        break;
      case 1:
        analogWrite(HEARTPIN, heartBrightness);
        heartCounter += 144/24.0;
        heartBrightness -= 255/24.0;
        if (heartBrightness < 0)
          heartBrightness = 0;
        break;
      case 2:
        analogWrite(HEARTPIN, heartBrightness);
        heartCounter += 144/12.0;
        heartBrightness += 255/12.0;
        if (heartBrightness > 255)
          heartBrightness = 255;
        break;
      case 3:
        analogWrite(HEARTPIN, heartBrightness);
        heartCounter += 144/72.0;
        heartBrightness -= 255/72.0;
        if (heartBrightness < 0)
          heartBrightness = 0;
        break;
    }
  }
    
 
  // drive blink pin high or low, depending on how much time has passed
  blinkDelay--;
  if (blinkDelay == 0) {
    blinkDelay = blinkTime;
    blinkState = !blinkState;
    digitalWrite(blinkPin, blinkState);
  }  
  
  // increase/decrease brightness of fade pin in steady incremental steps
  analogWrite(fadePin, fadeBrightness);
  fadeBrightness += fadeStep * fadeDirection;
  if (fadeBrightness >= 255) {
    fadeBrightness = 255;
    fadeDirection = -1;
  }
  if (fadeBrightness <= 0) {
    fadeBrightness = 0;
    fadeDirection = 1; 
  }
}

// calculate ratio of time LED is on/off to acheive particular brightness
int calcTimeOn(int desiredBrightness) {
  return desiredBrightness / maxBrightness * maxTimeOn;
}


// restart a new twinkle 
void startOver(){
  if (random(0, 1) == 1)
    targetBrightness = currentBrightness + random(100, 200);
  else
    targetBrightness = currentBrightness - random(100, 200);
  if (targetBrightness < 0)
    targetBrightness = maxBrightness + targetBrightness;
  targetBrightness %= (int)maxBrightness;
  timeToBright = random(10, 30); 
  stepToBright = (targetBrightness - currentBrightness) / timeToBright;
}
