#This script is intended to get
#the date and time using Python's data structures
import os,time

def shutdown():
  print "Shutting down"
  time.sleep(2)
  shutdown_bash = "shutdown -h now"
  os.system(shutdown_bash)

#if __name__=="__main__":
#  import datetime,time, os
#  import subprocess

#  while True:
#    now = datetime.datetime.now()
#    if now.hour ==  5 and now.minute == 28:
#      print "I am shutting down"
#      break

#shutdown_pi()  
