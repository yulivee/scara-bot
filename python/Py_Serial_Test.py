#This Test Code is for Use with the robot master to communicate via USB Serial.
#It must be run with an open console to print the results to

import serial, time, io

#open serial connection
arduino = serial.Serial('COM10', 115200, timeout=.1)
time.sleep(1) #give the connection a second to settle

arduino.write('0,-7,6,-5,4,-3,2,-1\n'.encode('utf-8')) #write command (In this case for the robot master to ping slaves)
arduino.flush() # it is buffering. required to get the data out *now*

#Main Loop
while True:
    b = arduino.readline() #read \n terminated byte array from arduino
    data = b.decode('utf-8') #convert to string
    if data:
        print(data.rstrip('\n')) #if there is something to print, print it!