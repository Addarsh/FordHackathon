/*
 * Code for the different sleep modes of the Arduino.
 * When the raspberry Pi is powered on, the Arduino is in sleep.
 * When the Pi shuts down, the Arduino wakes up from sleep and goes
 * into a new sleep mode where its wake up is triggered by a timer
 * interrupt.
 * Author: Addarsh Chandrasekar
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


#define LED_PIN (13)
#define BOOT_PIN 7
int pin2 = 2; //Pin used to trigger external interrupt

//Arduino under two different sleep modes,
//One where it wakes up on an external interrupt 
//Other one where it wakes up on timer interrupt
volatile int sleepMode = 0; //state: 0 - mode is external sleep
                            //state 1: mode is watchdog sleep
volatile int f_wdt=1;
int timeelapsed = 0;
int newdaytime = 24; //The Arduino timer interrupt reset time
volatile int int_count = 0;
/***************************************************
 *  Name:        ISR(WDT_vect)
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Watchdog Interrupt Service. This
 *               is executed when watchdog timed out.
 *
 ***************************************************/
ISR(WDT_vect)
{
  if(f_wdt == 0)
  {
    f_wdt=1;
  }
}

/***************************************************
 *  Name:        bootPi
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Restarts the raspberry pi
 *
 ***************************************************/
void bootPi(void){
  digitalWrite(BOOT_PIN,HIGH);
  delay(100);
  digitalWrite(BOOT_PIN,LOW);
}


/***************************************************
 *  Name:        enterSleepWatch
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleepWatch(void)
{
  //Renable watch timer
  WDTCSR |= _BV(WDIE);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();
  
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();

  timeelapsed += 8;

  if(timeelapsed == newdaytime){
    timeelapsed = 0;
    //BOOT the raspberry pi
    bootPi();
    
    sleepMode = 0; //Go back to inetrrupt mode
    delay(100);
  }
}



/***************************************************
 *  Name:        pin2Interrupt
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Service routine for pin2 interrupt  
 *
 ***************************************************/
void pin2Interrupt(void)
{ 
  /* We detach the interrupt to stop it from 
   * continuously firing while the interrupt pin
   * is low.
   */
  detachInterrupt(0);
  int_count++;
  if(int_count == 1){
    int_count = 0;
    sleepMode = 1;
  }
}


/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Enters the arduino into sleep mode.
 *
 ***************************************************/
void enterSleep(void)
{
  //Disable watch timer
   WDTCSR &= ~_BV(WDIE);
  
  //Attach interrupt to pin 2
  attachInterrupt(0, pin2Interrupt, LOW);
  delay(100);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  sleep_enable();

  //Enter sleep
  sleep_mode();
  
  
  //Disable sleep after exiting sleep mode
  sleep_disable();

  delay(200);
}


/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup watch dog timer 
 *
 ***************************************************/
void wdt_setup()
{
  
  // Clear the reset flag
  MCUSR &= ~(1<<WDRF);
  
  //Set WDCE to change prescalar
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  //Set watchdog timer prescalar so timeout = 8 s
  WDTCSR = 1<<WDP0 | 1<<WDP3; 
  
}


/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the Arduino.           
 *
 ***************************************************/
void setup()
{  
  //Set up pin directions
  pinMode(pin2, INPUT);
  pinMode(LED_PIN,OUTPUT);

  //Watch dog timer setup
  wdt_setup();

  //Set reset pin to output mode
  pinMode(BOOT_PIN,OUTPUT);
  digitalWrite(BOOT_PIN,LOW);

}



/***************************************************
 *  Name:        loop
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
int seconds=0;
void loop()
{
  if(sleepMode == 0){
    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    delay(1000);
    seconds++;

    //Wait for 2 more seconds before going to sleep
    if(seconds == 2)
    {
      delay(200);
      seconds = 0;
      enterSleep();
    }
  } else{
    if(f_wdt == 1)
    {
      //Toggle LED
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
      //Clear flag
      f_wdt = 0;
    
      //Enter sleep mode under watch dog timer interrupt
      enterSleepWatch();
    }
  }
  
}
