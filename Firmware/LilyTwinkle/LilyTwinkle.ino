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
boolean celebrationPeak = false; // Keep track what phase of the celebration phase we're in.

const int numberOfLEDs = 5;
int fadeTimer[numberOfLEDs] = { 10, 10, 10, 10, 10 };
byte onTime[numberOfLEDs] = { 0, 0, 0, 0, 0 };
byte onCounter[numberOfLEDs] = { 0, 0, 0, 0, 0 };
int fadeCounter[numberOfLEDs] = { 0, 0, 0, 0, 0 };
char dir[numberOfLEDs] = { 1, 1, 1, 1, 1 };
byte limit[numberOfLEDs] = { 255, 255, 255, 255, 255 };
boolean enable[numberOfLEDs] = { true, true, true, true, true };

long delayTime = 50; 
long startTime = 0;

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
    if ((waitingToCelebrate) && (enable[0] == false) && (enable[1] == false) && (enable[2] == false) && (enable[3] == false) && (enable[4] == false)) {
      celebrate = true;
      waitingToCelebrate = false;

      byte i;
      for (i = 0; i < numberOfLEDs; i++) {
        fadeTimer[i] = 500; // Set every LED to have a very slow next fade.
        onTime[i] = 0;  // reset pwm counter.
        fadeCounter[i] = 0;
        dir[i] = 1;
        limit[i] = 255;
        enable[i] = true;
      }
    }
    
    // LED0 section
    if (!enable[0])                  digitalWrite(LED0, LOW);
    else if (onCounter[0] > onTime[0]) digitalWrite(LED0, LOW);
    else                           digitalWrite(LED0, HIGH);

    onCounter[0]++;
    fadeCounter[0]++;

    if (fadeCounter[0] == fadeTimer[0])
    {
      fadeCounter[0] = 0;
      onTime[0] += dir[0];
      
      // if ((celebrate) && (!celebrationPeak) && (onTime[0] == limit[0])){
      //   celebrationPeak = true; // Initial celebration ramp-up completed.
      //                            // Commence next phase.
      //   delay(5000); // Hold
      //   celebrationPeak = false;
     // } else 
      if ((onTime[0] == limit[0]) || (onTime[0] == 0)) dir[0] *= -1;

      // End of a fade cycle when the LED was on.
      if ((onTime[0] == 0) && (dir[0] = 1))
      {
        limit[0] =     random(LIMITMIN0,LIMITMAX0); // pin-specific brightness values
        fadeTimer[0] = random(fadeMinDynamic0,fadeMaxDynamic0); // pin specific dynamic-fade speed variables
        
        if (celebrate) {
          // The celebration fade is over. That was fun.
          // Time to go home.
          celebrate = false;
          celebrationRoll = 500; // Next celebration in 500 cycles
        }

        if (waitingToCelebrate) {
          enable[0] = false;
        }
        else if (fastMode) {
          enable[0] = true;
        } 
        else {
          // As long as we're not waiting to celebrate,
          // and it's not fast mode,
          // then roll another die.
          enable[0] =  random(0,fadeTrueDynamic0+1) >= fadeFalseDynamic0;
        }
      }
    }
     
  //  LED1 section-----------------------------------------------------------------
    if (!enable[1])                   digitalWrite(LED1, LOW);
    else if (onCounter[1] > onTime[1]) digitalWrite(LED1, LOW);
    else                           digitalWrite(LED1, HIGH);
    onCounter[1]++;
    fadeCounter[1]++;
    if (fadeCounter[1] == fadeTimer[1])
    {
      fadeCounter[1] = 0;
      onTime[1] += dir[1];
      if ((onTime[1] == limit[1]) || (onTime[1] == 0)) dir[1] *= -1;
      if ((onTime[1] == 0) && (dir[1] = 1))
      {
        limit[1] =     random(LIMITMIN1,LIMITMAX1); // pin-specific brightness values
        fadeTimer[1] = random(fadeMinDynamic,fadeMaxDynamic);


        if (waitingToCelebrate) {
          enable[1] = false;
        }
        else if (fastMode) {
          enable[1] = true;
        } 
        else {
          enable[1] = random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }

        // fade-cycle completions counter.
        // Only triggers at the end of a full fade cycle when the LED was on.
        if (enable[1]) {

          // Only increment the fast mode counter if celebration-mode flags are false
          if ((!celebrate) && (!waitingToCelebrate)) {
            pin1FadeCycleCompletionCount++;
          }

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
            byte i;
            for (i = 0; i < numberOfLEDs; i++) {
              enable[i] = true; // turn every LED on.
            }
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
            fastModeCycleCountTrigger = random(FASTMODECYCLETRIGGERMIN, FASTMODECYCLETRIGGERMAX); // How many LED1 fade cycles until fast mode again?
          }
          
        }

        // After every fade cycle, increment celebration dice roll
        celebrationRoll++;
        // Now we celebrate
        if (celebrationRoll == 25) {
          waitingToCelebrate = true; // Inform everyone it's time to gather for the celebration!
        }
      }
    }

  //  LED2 section-----------------------------------------------------------------
    if (!enable[2])                   digitalWrite(LED2, LOW);
    else if (onCounter[2] > onTime[2]) digitalWrite(LED2, LOW);
    else                           digitalWrite(LED2, HIGH);
    onCounter[2]++;
    fadeCounter[2]++;
    if (fadeCounter[2] == fadeTimer[2])
    {
      fadeCounter[2] = 0;
      onTime[2] += dir[2];
      if ((onTime[2] == limit[2]) || (onTime[2] == 0)) dir[2] *= -1;
      if ((onTime[2] == 0) && (dir[2] = 1))
      {
        limit[2] =     random(LIMITMIN,LIMITMAX);
        fadeTimer[2] = random(fadeMinDynamic,fadeMaxDynamic);

        if (waitingToCelebrate) {
          enable[2] = false;
        }
        else if (fastMode) {
          enable[2] = true;
        } 
        else {
          enable[2] =  random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }
        
      }
    }
  
  //  LED3 section-----------------------------------------------------------------
    if (!enable[3])                   digitalWrite(LED3, LOW);
    else if (onCounter[3] > onTime[3]) digitalWrite(LED3, LOW);
    else                           digitalWrite(LED3, HIGH);
    onCounter[3]++;
    fadeCounter[3]++;
    if (fadeCounter[3] == fadeTimer[3])
    {
      fadeCounter[3] = 0;
      onTime[3] += dir[3];
      if ((onTime[3] == limit[3]) || (onTime[3] == 0)) dir[3] *= -1;
      if ((onTime[3] == 0) && (dir[3] = 1))
      {
        limit[3] =     random(LIMITMIN,LIMITMAX);
        fadeTimer[3] = random(fadeMinDynamic,fadeMaxDynamic);

        if (waitingToCelebrate) {
          enable[3] = false;
        }
        else if (fastMode) {
          enable[3] = true;
        } 
        else {
          enable[3] = random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }
        
      }
    }

  //  LED4 section-----------------------------------------------------------------
    if (!enable[4])                   digitalWrite(LED4, LOW);
    else if (onCounter[4] > onTime[4]) digitalWrite(LED4, LOW);
    else                           digitalWrite(LED4, HIGH);
    onCounter[4]++;
    fadeCounter[4]++;
    if (fadeCounter[4] == fadeTimer[4])
    {
      fadeCounter[4] = 0;
      onTime[4] += dir[4];
      if ((onTime[4] == limit[4]) || (onTime[4] == 0)) dir[4] *= -1;
      if ((onTime[4] == 0) && (dir[4] = 1))
      {
        limit[4] =     random(LIMITMIN,LIMITMAX);
        fadeTimer[4] = random(fadeMinDynamic,fadeMaxDynamic);

        if (waitingToCelebrate) {
          enable[4] = false;
        }
        else if (fastMode) {
          enable[4] = true;
        } 
        else {
          enable[4] = random(0,fadeTrueDynamic+1) >= fadeFalseDynamic;
        }
        
      }
    }
  }
}
