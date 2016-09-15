#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/wdt.h>


#define LED_PIN (13)
#define REBOOT_PIN 7
int pin2 = 2;
volatile int sleepMode = 0; //0 means mode is external sleep
                   //1 means mode is watchdog sleep
volatile int f_wdt=1;
int timeelapsed = 0;
int newdaytime = 24;
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
 *  Name:        rebootPi
 *
 *  Returns:     Nothing.
 *
 *  Parameters:  None.
 *
 *  Description: Resets the raspberry pi
 *
 ***************************************************/
void rebootPi(void){
  digitalWrite(REBOOT_PIN,HIGH);
  delay(100);
  digitalWrite(REBOOT_PIN,LOW);
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
    //Reboot the raspberry pi
    rebootPi();
    
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
  /* This will bring us back from sleep. */
  
  /* We detach the interrupt to stop it from 
   * continuously firing while the interrupt pin
   * is low.
   */
  detachInterrupt(0);
  int_count++;
  //Serial.println("ok");
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
  
  /* Setup pin2 as an interrupt and attach handler. */
  attachInterrupt(0, pin2Interrupt, LOW);
  delay(100);
  
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  
  sleep_enable();
 
  sleep_mode();
  
  /* The program will continue from here. */
  
  /* First thing to do is disable sleep. */
  sleep_disable();

  /*Serial.println("ow");
  Serial.print(int_count);
  Serial.print("\n");*/

  delay(200);
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
void wdt_setup()
{
  /*** Setup the WDT ***/
  
  /* Clear the reset flag. */
  MCUSR &= ~(1<<WDRF);
  
  /* In order to change WDE or the prescaler, we need to
   * set WDCE (This will allow updates for 4 clock cycles).
   */
  WDTCSR |= (1<<WDCE) | (1<<WDE);

  /* set new watchdog timeout prescaler value */
  WDTCSR = 1<<WDP0 | 1<<WDP3; /* 8.0 seconds */
  
  /* Enable the WD interrupt (note no reset). */
  //WDTCSR |= _BV(WDIE);
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
  /* Setup the pin direction. */
  pinMode(pin2, INPUT);
  pinMode(LED_PIN,OUTPUT);

  wdt_setup();

  //Set reset pin to output mode
  pinMode(REBOOT_PIN,OUTPUT);
  digitalWrite(REBOOT_PIN,LOW);

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
  
    if(seconds == 2)
    {
      delay(200);
      seconds = 0;
      enterSleep();
    }
  } else{
    if(f_wdt == 1)
    {
      /* Toggle the LED */
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    
      /* Don't forget to clear the flag. */
      f_wdt = 0;
    
      /* Re-enter sleep mode. */
      enterSleepWatch();
    }
  }
  
}
