// AVRDUDE fuse settings string:
// avrdude -p t85 -c avrispmkii -B 4 -P usb -U lfuse:w:0xe2:m -U hfuse:w:0xdf:m -U efuse:w:0xff:m
// By the time most users see this board, these settings will already 
//  have been applied. However, when programming from a blank ATTiny, it
//  is necessary to program the fuse bits to disable the divide-by-8
//  on the clock frequency.

// #define LED0 0
// #define LED1 1
// #define LED2 2
// #define LED3 3
// #define LED4 4
#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3
#define LED4 7

#define FADEMIN   50
#define FADEMAX   55
#define FADEFALSE 25
#define FADETRUE  30
#define LIMITMIN  125
#define LIMITMAX  255

// Restricting LED groups 0 and 1 to a minimum 90% PWM duty cycle ensurs that
// the NOT gate LEDs actually turn off during the fade loop.
#define LIMITMIN0 230    // LED 0 brightness limit
#define LIMITMAX0 255
#define LIMITMIN1 230    // LED 1 brightness limit
#define LIMITMAX1 255


#define FADEMIN0   150  // LED0 gets a different fade rate than everyone else.
#define FADEMAX0   355 
#define FADEFALSE0   10  // LED0 rolls enable more often
#define FADETRUE0    30

// Fast mode stuff -------------------------------------
#define FADEMINFAST 20 // Alternative "fast-mode" fade rate. Should be fast!
#define FADEMAXFAST 30
#define FADEFALSEFAST 15   // Fast mode-dice rolls. Should make more bright :D
#define FADETRUEFAST  30

#define FASTMODECYCLETRIGGERMIN 3   //  Used in a random number generator. Minimum fade cycles until next fast mode
#define FASTMODECYCLETRIGGERMAX 20  // Maximum fade cycles until next random fast mode

// Fast mode variables. Initialize frequency and duration dynamic variables from constants.
// Will be changed to faster values upon a trigger.
int fadeFalseDynamic = FADEFALSE;
int fadeTrueDynamic = FADETRUE;
int fadeMinDynamic = FADEMIN;
int fadeMaxDynamic = FADEMAX;
int fadeMinDynamic0 = FADEMIN; // LED0 has a different default fade speed range (slower, longer) than the rest.
int fadeMaxDynamic0 = FADEMAX; // But start out with the default rate so LED0 has a better chance to get coffee with the rest (the first time)
int fadeFalseDynamic0 = FADEFALSE0; // LED0 has a higher probability of rolling enable.
int fadeTrueDynamic0 = FADETRUE0; 


// After how many fade cycles will the first fast mode trigger?
int fastModeCycleCountTrigger = random(3,5); // Make sure fast mode triggers quickly the first time after opening the box.
int fastMode = false; // Are we in fast mode?
// End fast mode -----------------------------------------

// Variables for fade cycle counter. Allows an event to trigger after x fade cycles
int pin1FadeCycleCompletionCount = 0;

// Variables for celebration fade.
boolean waitingToCelebrate = false; // Are we waiting to celebrate?
boolean celebrate = false; // Are we celebrating right now?
int celebrationRoll = 0;  // Holds the magic number to match for celebrations.


const int numberOfLEDs = 5;
int fadeTimer[numberOfLEDs] = { 10, 10, 10, 10, 10 };

long delayTime = 50; 
long startTime = 0;

byte onTime0 = 0;
byte onCounter0 = 0; 
byte limit0 = 255;
char dir0 = 1;
boolean enable0 = true;
int fadeTimer0 = 10;
int fadeCounter0 = 0;

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
    startTime = currTime;

    // Check to see if we're waiting to celebrate, and if everyone has arrived.
    if ((waitingToCelebrate) && (enable0 == false) && (enable1 == false) && (enable2 == false) && (enable3 == false) && (enable4 == false)) {
      celebrate = true;
      waitingToCelebrate = false;

      for (i = 0; i < numberOfLEDs; i++) {
        fadeTimer[i] = 500; // Set every LED to have a very slow next fade.
      }

      // Reset all counters so that everyone fades from the start.
      onTime0 = 0;		// reset pwm counter.
      onTime1 = 0;
      onTime2 = 0;
      onTime3 = 0;
      onTime4 = 0;
      onCounter0 = 0;		// Say we've been on for 0 time.
      onCounter1 = 0;
      onCounter2 = 0;
      onCounter3 = 0;
      onCounter4 = 0;
      fadeCounter0 = 0;	// Say we've been PWMing for no time
      fadeCounter1 = 0;
      fadeCounter2 = 0;
      fadeCounter3 = 0;
      fadeCounter4 = 0;
      dir0 = 1;	// Ensures the celebration fade direction is up.
      dir1 = 1;
      dir2 = 1;
      dir3 = 1;
      dir4 = 1;
      limit0 = 255; // Celebration fade can reach max brightness
      limit1 = 255;
      limit2 = 255;
      limit3 = 255;
      limit4 = 255;

      enable0 = true; // Everyone START CELEBRATING!!!!!!!!!!!!!
      enable1 = true;
      enable2 = true;
      enable3 = true;
      enable4 = true;
    }
    
    // LED0 section
    if (!enable0)                  digitalWrite(LED0, LOW);
    else if (onCounter0 > onTime0) digitalWrite(LED0, LOW);
    else                           digitalWrite(LED0, HIGH);

    onCounter0++;
    fadeCounter0++;

    if (fadeCounter0 == fadeTimer[0])
    {
      fadeCounter0 = 0;
      onTime0 += dir0;
      
      if ((onTime0 == limit0) || (onTime0 == 0)) dir0 *= -1;
      if ((onTime0 == 0) && (dir0 = 1))
      {
        limit0 =     random(LIMITMIN0,LIMITMAX0); // pin-specific brightness values
        fadeTimer[0] = random(fadeMinDynamic0,fadeMaxDynamic0); // pin specific dynamic-fade speed variables
        
        if (waitingToCelebrate) {
          enable0 = false;
        }
        else if (fastMode) {
          enable0 = true;
        } 
        else {
          // As long as we're not waiting to celebrate,
          // and it's not fast mode,
          // then roll another dice.
          enable0 =  random(0,fadeTrueDynamic0+1) >= fadeFalseDynamic0;
        }
      }
    }
     
  //  LED1 section-----------------------------------------------------------------
    if (!enable1)                   digitalWrite(LED1, LOW);
    else if (onCounter1 > onTime1) digitalWrite(LED1, LOW);
    else                           digitalWrite(LED1, HIGH);
    onCounter1++;
    fadeCounter1++;
    if (fadeCounter1 == fadeTimer[1])
    {
      fadeCounter1 = 0;
      onTime1 += dir1;
      if ((onTime1 == limit1) || (onTime1 == 0)) dir1 *= -1;
      if ((onTime1 == 0) && (dir1 = 1))
      {
        limit1 =     random(LIMITMIN1,LIMITMAX1); // pin-specific brightness values
        fadeTimer[1] = random(fadeMinDynamic,fadeMaxDynamic);


        if (waitingToCelebrate) {
          enable1 = false;
        }
        else if (fastMode) {
          enable1 = true;
        } 
        else {
          enable1 = random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }

        // fade-cycle completions counter.
        // Only triggers at the end of a full fade cycle when the LED was on.
        if (enable1) {

          // If we're celebrating then don't bother drinking any coffee.
          if ((!celebrate) && (!waitingToCelebrate)) {
            pin1FadeCycleCompletionCount++;

            if (pin1FadeCycleCompletionCount == fastModeCycleCountTrigger) {
            // ON THE nth FADE
            // Give everyone coffee!!!!!!!!!!!!!!!!!!!!!!!
            fadeTrueDynamic = FADETRUEFAST;
            fadeFalseDynamic = FADEFALSEFAST;
            fadeMinDynamic = FADEMINFAST;
            fadeMaxDynamic = FADEMAXFAST;
            fadeMinDynamic0 = FADEMINFAST;  // pin-specific dynamic fade speed variable
            fadeMaxDynamic0 = FADEMAXFAST;
            fadeFalseDynamic0 = FADEFALSEFAST;  // pin-specific dynamic dice roll
            fadeTrueDynamic0 = FADETRUEFAST;
            fadeTimer[0] = random(fadeMinDynamic,fadeMaxDynamic); // Hijack LED0's fade timer just in case it's
                          // in the middle of a really long fade. Replace it with something short. That way it
                          // will coffee too!!!!
            enable0 = true;
            enable1 = true;
            enable2 = true;
            enable3 = true;
            enable4 = true;
            }
            if (pin1FadeCycleCompletionCount == 11) {
              // Uh oh coffee has worn off eveyrone is sleepy.
              fadeTrueDynamic = FADETRUE;
              fadeFalseDynamic = FADEFALSE;
              fadeMinDynamic = FADEMIN;
              fadeMaxDynamic = FADEMAX;
              fadeMinDynamic0 = FADEMIN0; // pin-specific dynamic fade speed variable
              fadeMaxDynamic0 = FADEMAX0;
              pin1FadeCycleCompletionCount = 0;
              fastModeCycleCountTrigger = random(FASTMODECYCLETRIGGERMIN, FASTMODECYCLETRIGGERMAX); // After how many loops will fast mode trigger again?
            }
          }
        }
        // After every fade cycle, increment celebration dice roll
        celebrationRoll++;
        // Now we celebrate
        if (celebrationRoll == 25) {
          waitingToCelebrate = true; // Inform everyone to gather for the celebration!
        }
      }
    }

  //  LED2 section-----------------------------------------------------------------
    if (!enable2)                   digitalWrite(LED2, LOW);
    else if (onCounter2 > onTime2) digitalWrite(LED2, LOW);
    else                           digitalWrite(LED2, HIGH);
    onCounter2++;
    fadeCounter2++;
    if (fadeCounter2 == fadeTimer[2])
    {
      fadeCounter2 = 0;
      onTime2 += dir2;
      if ((onTime2 == limit2) || (onTime2 == 0)) dir2 *= -1;
      if ((onTime2 == 0) && (dir2 = 1))
      {
        limit2 =     random(LIMITMIN,LIMITMAX);
        fadeTimer[2] = random(fadeMinDynamic,fadeMaxDynamic);

        if (waitingToCelebrate) {
          enable2 = false;
        }
        else if (fastMode) {
          enable2 = true;
        } 
        else {
          enable2 =  random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }
        
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
        fadeTimer[3] = random(fadeMinDynamic,fadeMaxDynamic);

        if (waitingToCelebrate) {
          enable3 = false;
        }
        else if (fastMode) {
          enable3 = true;
        } 
        else {
          enable3 = random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }
        
      }
    }

  //  LED4 section-----------------------------------------------------------------
    if (!enable4)                   digitalWrite(LED4, LOW);
    else if (onCounter4 > onTime4) digitalWrite(LED4, LOW);
    else                           digitalWrite(LED4, HIGH);
    onCounter4++;
    fadeCounter4++;
    if (fadeCounter4 == fadeTimer[4])
    {
      fadeCounter4 = 0;
      onTime4 += dir4;
      if ((onTime4 == limit4) || (onTime4 == 0)) dir4 *= -1;
      if ((onTime4 == 0) && (dir4 = 1))
      {
        limit4 =     random(LIMITMIN,LIMITMAX);
        fadeTimer[4] = random(fadeMinDynamic,fadeMaxDynamic);

        if (waitingToCelebrate) {
          enable4 = false;
        }
        else if (fastMode) {
          enable4 = true;
        } 
        else {
          enable4 = random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }
        
      }
    }
  }
}
