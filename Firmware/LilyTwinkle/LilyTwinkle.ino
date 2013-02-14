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

#define FADEMIN   50
#define FADEMAX   55
#define FADEFALSE 25
#define FADETRUE  30
#define LIMITMIN  125
#define LIMITMAX  255

long delayTime = 50;
long startTime = 0;

byte onTime0 = 0;
byte onCounter0 = 0;
byte limit0 = 255;
char dir0 = 1;
byte enable0 = 1;
int fadeTimer0 = 10;
int fadeCounter0 = 0;

byte onTime1 = 40;
byte onCounter1 = 0;
byte limit1 = 255;
char dir1 = 1;
byte enable1 = 1;
int fadeTimer1 = 20;
int fadeCounter1 = 0;

byte onTime2 = 80;
byte onCounter2 = 0;
byte limit2 = 255;
char dir2 = 1;
byte enable2 = 1;
int fadeTimer2 = 30;
int fadeCounter2 = 0;

byte onTime3 = 120;
byte onCounter3 = 0;
byte limit3 = 255;
char dir3 = 1;
byte enable3 = 1;
int fadeTimer3 = 40;
int fadeCounter3 = 0;

void setup()  { 
  pinMode(LED0, OUTPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  startTime = micros();
} 

void loop()
{ 
  long currTime = micros();
  if ( (currTime - startTime) > delayTime)
  {
    startTime = currTime;
  //  LED0 section-----------------------------------------------------------------
    if (enable0 <= FADEFALSE)      digitalWrite(LED0, LOW);
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
        limit0 =     random(LIMITMIN,LIMITMAX);
        fadeTimer0 = random(FADEMIN,FADEMAX);
        enable0 =    random(0,FADETRUE+1);
      }
    }
     
  //  LED1 section-----------------------------------------------------------------
    if (enable1 <= FADEFALSE)      digitalWrite(LED1, LOW);
    else if (onCounter1 > onTime1) digitalWrite(LED1, LOW);
    else                           digitalWrite(LED1, HIGH);
    onCounter1++;
    fadeCounter1++;
    if (fadeCounter1 == fadeTimer1)
    {
      fadeCounter1 = 0;
      onTime1 += dir1;
      if ((onTime1 == limit1) || (onTime1 == 0)) dir1 *= -1;
      if ((onTime1 == 0) && (dir1 = 1))
      {
        limit1 =     random(LIMITMIN,LIMITMAX);
        fadeTimer1 = random(FADEMIN,FADEMAX);
        enable1 =    random(0,FADETRUE+1);
      }
    }

  //  LED2 section-----------------------------------------------------------------
    if (enable2 <= FADEFALSE)      digitalWrite(LED2, LOW);
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
        fadeTimer2 = random(FADEMIN,FADEMAX);
        enable2 =    random(0,FADETRUE+1);
      }
    }
  
  //  LED3 section-----------------------------------------------------------------
    if (enable3 <= FADEFALSE)      digitalWrite(LED3, LOW);
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
        fadeTimer3 = random(FADEMIN,FADEMAX);
        enable3 =    random(0,FADETRUE+1);
      }
    }
  }
}
