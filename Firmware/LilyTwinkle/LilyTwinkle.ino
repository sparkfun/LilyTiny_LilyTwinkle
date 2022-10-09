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
#define LED4 4

#define FADEMIN   50   // FADEMIN and FADEMAX control the rate of fading. The wider
#define FADEMAX   55   //  the spread here, the greater the difference between
                       //  fastest fade and slowest fade. FADEMIN sets the lower
                       //  limit; the smaller that number gets, the faster the fade
                       //  will be allowed to be. 
#define FADEMINFAST 10 // Alternative "fast-mode" fade rate. Should be fast!
#define FADEMAXFAST 20
#define FADEMIN0   150  // LED0 gets a different fade rate than everyone else.
#define FADEMAX0   355   

#define FADEFALSE 25   // FADEFALSE and FADETRUE are a sort of dice-roll to decide
#define FADETRUE  30   //  whether an LED will be on for this cycle or not. Every
                       //  time an LED hits zero brightness, a random number between
                       //  0 and FADETRUE + 1 is created. If that number is greater
                       //  than FADEFALSE, the LED will go through a twinkle cycle.
                       //  If it's less than FADEFALSE, the code will still run as
                       //  though a twinkle is occuring, but the LED won't come on.
#define FADEFALSEFAST 7   // Fast mode-dice rolls. Should make more bright :D
#define FADETRUEFAST  30   //  

#define FADEFALSE0  5   // LED-specific fade rates.
#define FADETRUE0   30
#define FADEFALSE4 95
#define FADETRUE4 100

#define LIMITMIN  125  // LIMITMIN and LIMITMAX are the limits on the brightest the
#define LIMITMAX  255  //  LED can get.

// Restricting LED groups 0 and 1 to a minimum 90% PWM duty cycle ensurs that
// the NOT gate LEDs actually turn off during the fade loop.
#define LIMITMIN0 230    // LED 0 brightness limit
#define LIMITMAX0 255
#define LIMITMIN1 230    // LED 1 brightness limit
#define LIMITMAX1 255

// Number of cycles to hold at LIMITMAX before reversing.
#define FADEHOLDMAX0 50

// Variables for debugging
boolean debugLEDState = false;  // keep track of LED 4 so we can toggle it on and off with PWM.
//

// Variables for fade cycle counter. Allows an event to trigger after x fade cycles
int countLED1sFadeCycles = 0;
//

// Fast mode variables. Initialize frequency and duration dynamic variables from constants.
// Can be changed on a trigger.
int fadeFalseDynamic = FADEFALSE;
int fadeTrueDynamic = FADETRUE;
int fadeMinDynamic = FADEMIN;
int fadeMaxDynamic = FADEMAX;
//

long delayTime = 50;   // Sets the loop speed, in microseconds.
long startTime = 0;    // Counter, for tracking loop iterations.


byte onTime0 = 0;      // Time an LED should be on before turning off for PWM
byte onCounter0 = 0;   // How long the LED *has* been on for PWM
byte limit0 = 255;     // The point at which the LED's onCounter resets to 0
char dir0 = 1;         // Are we getting brighter (1), or dimmer (-1)?
boolean enable0 = true;// Results for the current "dice roll"
int fadeTimer0 = 10;   // How long should we PWM before increasing onTime?
int fadeCounter0 = 0;  // How long have we been PWMing since last onTime increase?
byte fadehold0 = 0;    // How long have we held at fade limit?

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

byte onTime4 = 0;
byte onCounter4 = 0;
byte limit4 = 255;
char dir4 = 1;
boolean enable4 = true;
int fadeTimer4 = 10;
int fadeCounter4 = 0;


void setup()  { 
  randomSeed(analogRead(3));
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  startTime = micros();
} 

void loop()
{ 
  long currTime = micros();
  if ( (currTime - startTime) > delayTime)
  {
    startTime = currTime;   // Record the last time we entered the loop.
    
  //  LED0 section-----------------------------------------------------------------
    if (!enable0)                  digitalWrite(LED0, LOW);

    else if (onCounter0 > onTime0) digitalWrite(LED0, LOW);
    else                           digitalWrite(LED0, HIGH);
    onCounter0++; 
    fadeCounter0++;
    if (fadeCounter0 == fadeTimer0)
    {
      fadeCounter0 = 0;
      onTime0 += dir0;
      
      if ((onTime0 == limit0) || (onTime0 == 0)) dir0 *= -1;

      if ((onTime0 == 0) && (dir0 = 1))
      {
        limit0 =     random(LIMITMIN0,LIMITMAX0);
        fadeTimer0 = random(FADEMIN0,FADEMAX0);
        enable0 =  random(0,FADETRUE0+1) >= FADEFALSE0;
      }
    }
     
  //  LED1 section-----------------------------------------------------------------
    if (!enable1) {
      digitalWrite(LED1, LOW);
    }
    else if (onCounter1 > onTime1) {
      digitalWrite(LED1, LOW);
    }

    // true + false many times a fade cycle. This is the PWM part.
    else {
      digitalWrite(LED1, HIGH);
    }

    onCounter1++;
    fadeCounter1++;

    if (fadeCounter1 == fadeTimer1)
    {
      fadeCounter1 = 0;
      onTime1 += dir1;

      // Identifies any change in fade direction
      if ((onTime1 == limit1) || (onTime1 == 0)) { 
        dir1 *= -1;
      }

      // This conditional is true many times when the LED is off.
      // The whole routine runs even if the LED is not enabled.
      if ((onTime1 == 0) && (dir1 = 1))
      {

        // True only at the very end of a full fade-cycle for an LED
        if (enable1) {
          // do magic.

          // Everyone celebrates the majesty of LED1's fade just past~!
          //enable0 = true;      
          // enable1 = true;
          // enable2 = true;      
          // enable3 = true;
          // enable4 = true;

          countLED1sFadeCycles++;  // count the number of completed fades.

          if (countLED1sFadeCycles == 3) {
            // ON THE THIRD FADE
            // Give everyone coffee!!!!!!!!!!!!!!!!!!!!!!!
            fadeTrueDynamic = FADETRUEFAST;
            fadeFalseDynamic = FADEFALSEFAST;
            fadeMinDynamic = FADEMINFAST;
            fadeMaxDynamic = FADEMAXFAST;
          }
          if (countLED1sFadeCycles == 11) {
            // Uh oh coffee has worn off eveyrone is sleepy.
            fadeTrueDynamic = FADETRUE;
            fadeFalseDynamic = FADEFALSE;
            fadeMinDynamic = FADEMIN;
            fadeMaxDynamic = FADEMAX;
            countLED1sFadeCycles = 0;
          }
        } 

        limit1 =     random(LIMITMIN1,LIMITMAX1);
        fadeTimer1 = random(fadeMinDynamic,fadeMaxDynamic);
        enable1 =    random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;

      }
    }

  //  LED2 section-----------------------------------------------------------------
    if (!enable2)                   digitalWrite(LED2, LOW);
    else if (onCounter2 > onTime2) digitalWrite(LED2, LOW);
    else                           digitalWrite(LED2, HIGH);
    onCounter2++;
    fadeCounter2++;
    if (fadeCounter2 == fadeTimer2)
    {
      fadeCounter2 = 0;
      onTime2 += dir2;
      if ((onTime2 == limit2) || (onTime2 == 0)) dir2 *= -1;
      if ((onTime2 == 0) && (dir2 = 1))
      {
        limit2 =     random(LIMITMIN,LIMITMAX);
        fadeTimer2 = random(fadeMinDynamic,fadeMaxDynamic);           // Dynamic fade allows for coffee mode.
        enable3 =    random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
      }
    }
  
  //  LED3 section-----------------------------------------------------------------
    if (!enable3)                   digitalWrite(LED3, LOW);
    else if (onCounter3 > onTime3) digitalWrite(LED3, LOW);
    else                           digitalWrite(LED3, HIGH);
    onCounter3++;
    fadeCounter3++;
    if (fadeCounter3 == fadeTimer3)
    {
      fadeCounter3 = 0;
      onTime3 += dir3;
      if ((onTime3 == limit3) || (onTime3 == 0)) dir3 *= -1;
      if ((onTime3 == 0) && (dir3 = 1))
      {
        limit3 =     random(LIMITMIN,LIMITMAX);
        fadeTimer3 = random(fadeMinDynamic,fadeMaxDynamic);           // Dynamic fade allows for coffee mode.
        enable3 =    random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;

      }
    }

   //LED4 section-----------------------------------------------------------------
    // if (!enable4)                   digitalWrite(LED4, LOW);
    // else if (onCounter4 > onTime4) digitalWrite(LED4, LOW);
    // else                           digitalWrite(LED4, HIGH);
    // onCounter4++;
    // fadeCounter4++;
    // if (fadeCounter4 == fadeTimer4)
    // {
    //   fadeCounter4 = 0;
    //   onTime4 += dir4;
    //   if ((onTime4 == limit4) || (onTime4 == 0)) dir4 *= -1;
    //   if ((onTime4 == 0) && (dir4 = 1))
    //   {
    //     limit4 =     random(LIMITMIN,LIMITMAX);
    //     fadeTimer4 = random(FADEMIN,FADEMAX);
    //     enable4 = false;
    //   }
    // }
  }
}
