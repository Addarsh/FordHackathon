/*
 * Sketch for testing sleep mode with wake up on WDT.
 * Donal Morrissey - 2011.
 *
 */
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>

#define LED_PIN (13)

volatile int f_wdt=1;
int pin2 = 2;
int sleepmode = 0; //0 for interupt sleep, 1 for watch sleep
int newdaytime = 32; //32 seconds is new day for our code, will be in sleep until then
int timeElapsed = 0;
bool firsttime = false;

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
  else
  {
    Serial.println("WDT Overrun!!!");
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
  /* This will bring us back from sleep. */
  
  /* We detach the interrupt to stop it from 
   * continuously firing while the interrupt pin
   * is low.
   */
  detachInterrupt(0);
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
void enterSleepWatchDog(void)
{
  set_sleep_mode(SLEEP_MODE_PWR_SAVE);   /* EDIT: could also use SLEEP_MODE_PWR_DOWN for lowest power consumption. */
  sleep_enable();
  
  /* Now enter sleep mode. */
  sleep_mode();

  timeElapsed += 8;
  /* The program will continue from here after the WDT timeout*/
  sleep_disable(); /* First thing to do is disable sleep. */
  
  /* Re-enable the peripherals. */
  power_all_enable();

  Serial.println("Watchdog awakes!, initialize interrupt!");

  delay(100);

  if(timeElapsed == newdaytime){
    //disable the watchdog timer
    WDTCSR &= ~_BV(WDIE);
    
    //Restart the rpi
    timeElapsed = 0; //Reset  time elapsed
    int_setup();
    delay(100);

//Back to interrupt driven timer
    sleepmode = 0; 
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
void enterSleepInterrupt(void)
{
  
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pin2Interrupt, LOW);
  delay(100);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  sleep_enable();
  
  sleep_mode();
  
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable(); 

  Serial.println("Intrr Sleep over!\n");

  
 
  //Setup the watchdog timer sleep mode
  wdt_setup();

  delay(100);
  sleepmode = 1; //Change sleep mode to watchdog type
}


void wdt_setup(){
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  WDTCSR |= _BV(WDIE);
  
  Serial.println("Initialisation complete.");
}


//Setup pin2 for interrupt
void int_setup(){
  /* Setup the pin direction. */
  pinMode(pin2, INPUT);
}


/***************************************************
 *  Name:        setup
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Setup for the serial comms and the
 *                Watch dog timeout. 
 *
 ***************************************************/
void setup()
{
  Serial.begin(9600);
  Serial.println("Initialising...");
  delay(100); //Allow for serial print to complete.

  pinMode(LED_PIN,OUTPUT);

  //Start off with the interu=rupt setup
  int_setup();

  /*** Setup the WDT ***/
  delay(100); //Allow for serial print to complete.
}



/***************************************************
 *  Name:        enterSleep
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Main application loop.
 *
 ***************************************************/
void loop()
{
  if (sleepmode == 1){
    if(f_wdt == 1)
    {
      Serial.println("Entering watchdog timer mode!");
      /* Toggle the LED */
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
      /* Don't forget to clear the flag. */
      f_wdt = 0;
    
      /* Re-enter sleep mode. */
      enterSleepWatchDog();
    }
  } else{
       Serial.println("Entering interrupt sleep mode!");
      /* Enter Interrupt sleep mode*/
      enterSleepInterrupt();
  }  
}
