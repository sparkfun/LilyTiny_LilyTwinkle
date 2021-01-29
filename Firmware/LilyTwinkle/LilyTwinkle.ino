// AVRDUDE fuse settings string:
// avrdude -p t85 -c avrispmkii -B 4 -P usb -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
// By the time most users see this board, these settings will already 
//  have been applied. However, when programming from a blank ATTiny, it
//  is necessary to program the fuse bits to disable the divide-by-8
//  on the clock frequency.

#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3

#define FADEMIN   50   // FADEMIN and FADEMAX control the rate of fading. The wider
#define FADEMAX   55   //  the spread here, the greater the difference between
                       //  fastest fade and slowest fade. FADEMIN sets the lower
                       //  limit; the smaller that number gets, the faster the fade
                       //  will be allowed to be. 
#define FADEFALSE 25   // FADEFALSE and FADETRUE are a sort of dice-roll to decide
#define FADETRUE  30   //  whether an LED will be on for this cycle or not. Every
                       //  time an LED hits zero brightness, a random number between
                       //  0 and FADETRUE + 1 is created. If that number is greater
                       //  than FADEFALSE, the LED will go through a twinkle cycle.
                       //  If it's less than FADEFALSE, the code will still run as
                       //  though a twinkle is occuring, but the LED won't come on.
#define LIMITMIN  125  // LIMITMIN and LIMITMAX are the limits on the brightest the
#define LIMITMAX  255  //  LED can get.

long delayTime = 50;   // Sets the loop speed, in microseconds.
long startTime = 0;    // Counter, for tracking loop iterations.

byte onTime0 = 0;      // Time an LED should be on before turning off for PWM
byte onCounter0 = 0;   // How long the LED *has* been on for PWM
byte limit0 = 255;     // The point at which the LED's onCounter resets to 0
char dir0 = 1;         // Are we getting brighter (1), or dimmer (-1)?
boolean enable0 = true;// Results for the current "dice roll"
int fadeTimer0 = 10;   // How long should we PWM before increasing onTime?
int fadeCounter0 = 0;  // How long have we been PWMing since last onTime increase?

byte onTime1 = 0;
byte onCounter1 = 0;
byte limit1 = 255;
char dir1 = 1;
boolean enable1 = true;
int fadeTimer1 = 10;
int fadeCounter1 = 0;

byte onTime2 = 0;
byte onCounter2 = 0;
byte limit2 = 255;
char dir2 = 1;
boolean enable2 = true;
int fadeTimer2 = 10;
int fadeCounter2 = 0;

byte onTime3 = 0;
byte onCounter3 = 0;
byte limit3 = 255;
char dir3 = 1;
boolean enable3 = true;
int fadeTimer3 = 10;
int fadeCounter3 = 0;

void setup()  { 
  randomSeed(analogRead(3));
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  startTime = micros();
} 

void loop() { 
  long currTime = micros();
  // This is a software PWM thing. We invoke this loop every 50us, at the fastest,
  //  although it may happen less often than that.
  if ( (currTime - startTime) > delayTime)   {
    startTime = currTime;   // Record the last time we entered the loop.
    twinkleLED (LED0, onTime0, onCounter0, limit0, dir0, enable0, fadeTimer0, fadeCounter0);
    twinkleLED (LED1, onTime1, onCounter1, limit1, dir1, enable1, fadeTimer1, fadeCounter1);
    twinkleLED (LED2, onTime2, onCounter2, limit2, dir2, enable2, fadeTimer2, fadeCounter2);
    twinkleLED (LED3, onTime3, onCounter3, limit3, dir3, enable3, fadeTimer3, fadeCounter3);
  }
}    

void twinkleLED (int LED, byte onTime, byte onCounter, byte limit, char dir, boolean enable, int fadeTimer, int fadeCounter) {  
  //  Each of the four LEDx sections is exactly like the others, so I'll only
  //   comment this one. The idea is pretty simple: on each pass through the
  //   loop, we increment two counters for each LED. One of them, onCounter, tracks
  //   whether the LED is on or off for PWM purposes. The other, fadeCounter, tracks
  //   wether it's time to increase (or decrease) the amount of time the LED will
  //   remain on for the next cycle, thereby increasing (or decreasing) the LEDs
  //   apparent brightness.
    
  // This if/else controls on/off of the LED:
  // enable is an override- some percentage of the time, we don't want the LED
  //  to be on at all; otherwise, the LEDs will be on too much.
  if (!enable)
    digitalWrite(LED, LOW);
    // while onCounter < onTime, the LED should be on. This is the short loop,
    //  which effectively PWMs the LED.
  else if (onCounter > onTime0)
    digitalWrite(LED, LOW);
  else
    digitalWrite(LED, HIGH);

  onCounter++;    // Advance the PWM counter. The number of times we loiter on
                   //  a single brightness value before increasing or decreasing
                   //  it is determined by the fadeCounter/fadeTimer relationship
  fadeCounter++;  // Advance the fade counter. This value controls the rate at
                   //  which the fade occurs- the higher the value of fadeTimer,
                   //  the slower the twinkle. After each fade cycle, we pick a
                   //  new, random-ish value for fadeTimer, with limits set by
                     //  FADEMIN and FADEMAX above.
    

  if (fadeCounter == fadeTimer) {
    fadeCounter = 0;  // Reset the fade counter for the next fade cycle.
    onTime += dir;   // Increase or decrease onTime, depending on whether we're
                       //  increasing or decreasing brightness.

    // Change from increasing brightness to decreasing brightness, or vice versa.
    if ((onTime == limit) || (onTime == 0)) 
      dir *= -1;

    // When we hit bottom, but before we start counting back up, randomize our
    //  variables for the next time around.
    if ((onTime == 0) && (dir = 1)) {
      // limit is the point at which the LED stops getting brighter and starts to
      //  dim again.
      limit =     random(LIMITMIN,LIMITMAX);

      // fadeTimer controls how fast the fading occurs. The lower it is, the
      //  faster the fade. FADEMIN and FADEMAX are set to a nice value.
      fadeTimer = random(FADEMIN,FADEMAX);

      // enable is a sort of dice-roll for whether the LED will be on or not for
      //  the next cycle it runs through. The probability runs something like
      //  FADEFALSE/(FADETRUE+1); by default, that's (1+25)/(1+30)=25/31=83.87% or about 80% of the
      //  rolls should be negative. We want to force an enable if more than a
      //  couple of disables have passed.
      enable =  random(0,FADETRUE+1) >= FADEFALSE;
    }
  }
}
