"""
File:     pst.py (Port Send Time)

Purpose:  Set the time on the arduino board
          for use with the Time library found
          at http://playground.arduino.cc/Code/time

Usage:    % python pst.py

Effect:   Send message like "T1359713903" over the serial port
          That is the time string for Fri Feb  1 10:18:23 EST 2013

Config:   Set the variable "time_zone_offset" for the offset to GMT.  The
          value -5 works for US Eastern Standard Time

          Set the variable port to the usb port used by your Arduino
          board.  In Arduino IDE, go to Tools > Serial Port and note
          which port is checked.
          
Ref:      http://www.moosechips.com/2010/07/python-subprocess-module-examples/

Author:   J. Carlson, Feb 2, 2013
"""

import os, subprocess

# Settings
time_zone_offset = -5
port = "/dev/cu.usbserial-A900JEHX"

# Put stdout into pipe
proc = subprocess.Popen("date +%s", shell=True, stdout=subprocess.PIPE)
return_code = proc.wait()
# Read from pipe
for line in proc.stdout:
    time = int(line.rstrip())

# Output to terminal and port
print
os.system("date")
time += 60*60*time_zone_offset
print "Time:", time

cmd = "echo T" + str(time) + " > " + port
print "Cmd:", cmd
print
os.system(cmd)