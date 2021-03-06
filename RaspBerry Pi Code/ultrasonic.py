#Ford Hackathon project: Smart Parking lot detection
#Script to integrate ultrasonic sensors (used to detect parking spot)
#with a raspberry pi. The data from the sensors is sent over WiFi to 
#to a Web server via HTTP request.
#Author: Addarsh Chandrasekar

#Function to trigger interrupt on Arduino
#This would then lead to automatic shutdown
#of the raspberry pi 
def call_arduino():
  #Turn off LED
  GPIO.output(LED_Pin, False);
  
  #Make Arduino Pin Low
  print "Calling Arduino!"
  GPIO.output(Ardu_Pin, False);
  time.sleep(0.1) #1 second  delay
  GPIO.output(Ardu_Pin, True);
  time.sleep(0.1) #1 second  delay

#Shutdown the raspberry pi
def shutdown():
  print "Shutting down"
  time.sleep(2)
  shutdown_bash = "shutdown -h now"
  os.system(shutdown_bash)

#Initial setup of all GPIO pins
def setup(Ultra_pins):
  for (trig,echo) in Ultra_pins: 
    #GPIO pins for Ultra
    GPIO.setup(trig, GPIO.OUT)
    GPIO.setup(echo, GPIO.IN)

  #Setup Arduino pin to be Output
  GPIO.setup(Ardu_Pin,GPIO.OUT)
  #Also, set it to be HIGH from the start
  GPIO.output(Ardu_Pin, True);

  #Setup LED pin to be Output
  GPIO.setup(LED_Pin,GPIO.OUT)
  #Also, set it to be HIGH from the start
  GPIO.output(LED_Pin, True);

  #Wait for a few seconds
  time.sleep(1)

#Function to measure distance of closest obstacle from the 
#ultrasonic sensor
def measure_distance(trig,echo,sensor_index,moving_index): 
  GPIO.output(trig,False)
  time.sleep(0.1)

  #10 micro pulse is the ultrasonic trigger
  GPIO.output(trig,True)
  time.sleep(trig_pulseduration)
  GPIO.output(trig,False)

  #Set a max timeout for the while loop
  #Currently set to 1 sec
  timeout = time.time() + 1 
  while GPIO.input(echo) == 0:
    pulse_start = time.time()
    if pulse_start >= timeout:
      break

  #timeout = time.time() + 1 
  while GPIO.input(echo) == 1:
    pulse_end = time.time()
    if pulse_end >= timeout:
      break

  pulse_duration = pulse_end - pulse_start
  distance = pulse_duration * SOUND_CONSTANT
  distance = round(distance,2)

  #Print distance for debugging purposes
  print "Distance from sensor "+str(sensor_index)+" is:", distance, "cm"

  #Append new distance to moving average list
  if moving_index < len_mov_avg:
    mov_avg_lists[sensor_index].append(distance)
  else:
    #Remove first index
    del mov_avg_lists[sensor_index][0]
    
    #Shift the list and add new data to last index 
    mov_avg_lists[sensor_index].append(distance)

  print "Moving average list for sensor", sensor_index, mov_avg_lists[sensor_index]

#Function to send sensor data to web server
def send_to_server():
  for sensor_index in range(len(Ultra_pins)):
    #Compute the mean of the data
    mean_distance = sum(mov_avg_lists[sensor_index])/len_mov_avg
     
    print "Mean distance: ", mean_distance
    #Parameters to send out
    status = "Unknown"  
    id_baseindex = 20
    id = id_baseindex+sensor_index
 
    #Check whether object is present/uknown/absent
    if mean_distance <= thresh_present:
      status = "closed"
    elif mean_distance >= thresh_unkown:
      status = "open"

    data = json.dumps({"id":str(id),"status":str(status)})
    print data
    #Make a CURL request 
    print "Making curl request " + str(sensor_index)
    try:
      req = urllib2.Request(server_url,data,{"Content-Type":"application/json"})
      urllib2.urlopen(req,timeout=1)
      #print "Sending request"
    except:
      print "Bad request, try again"
    #time.sleep(1) #Wait for 1 second between each send


if __name__ == "__main__":
  import RPi.GPIO as GPIO
  import time,datetime,sys
  import urllib2,json
  from turnOffPi import shutdown

  #Set Arduino interrupt pin 
  Ardu_Pin = 26  
  LED_Pin = 19  

  if(len(sys.argv) < 3):
   print "Error! Too few arguments!"

  #Get the hour and minute  aguments
  turnoff_Hour = int(sys.argv[1])
  turnoff_Minute = int(sys.argv[2])
  

  #Some constants
  trig_pulseduration = 0.00001 #10 microseconds
  SOUND_CONSTANT = 17150 #340
  server_url = "http://52.41.133.252:3000/api/updateSpot"

  #The Ultrasonic sensors are on the following pins
  # Ultra 0 : TRIG = 18, ECHO = 15
  # Ultra 1 : TRIG = 14, ECHO = 4
  # Ultra 2 : TRIG = 17, ECHO = 27
  # Ultra 3 : TRIG = 6, ECHO = 5
  # Ultra 4 : TRIG = 19, ECHO = 13
  
  #Each tuple is of the form: (TRIG,ECHO)
  Ultra_pins = [(18,15),(14,4),(17,27),(6,5),(19,13)]

  #Set GPIO mod to BCM 
  GPIO.setmode(GPIO.BCM)

  #Setup the pins 
  setup(Ultra_pins)  #Setup the pi for comands

  #Setup moving averages for ultrasonic sensor values
  mov_avg_lists = [[] for i in range(len(Ultra_pins))]

  #Length of moving average list
  len_mov_avg = 5

  #Threshold values for Car detection
  thresh_present = 20 #20cm or below car is present
  thresh_unkown = 30 #between 20 and 30 cm, we dont know for sure


  start_time = time.time()
  moving_index = 0
  while True:
    #Check if it is time to shutdown
    now = datetime.datetime.now()
    if now.hour > turnoff_Hour: #After hour:time
      break
    elif now.hour == turnoff_Hour:
      if now.minute >= turnoff_Minute:
        break 

    sensor_num = 0;
    time_to_send = False
    for (trig,echo) in Ultra_pins:
      try:
        measure_distance(trig,echo,sensor_num,moving_index)
        #time.sleep(1)
        sensor_num += 1
      except (KeyboardInterrupt,SystemExit()):
        raise
      except:
        #No worries, keep going
        continue
    stop_time = time.time()
    
    if stop_time - start_time >= 10:
      #Send data to server every 10 seconds
      print "Sending to server!"
      send_to_server() 
      start_time = time.time()
    moving_index += 1

  call_arduino()
  GPIO.cleanup()
  shutdown() #Shutdown the raspberry pi

