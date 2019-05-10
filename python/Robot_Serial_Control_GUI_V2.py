#GUI librarys
from tkinter import *
from tkinter import messagebox
#Serial librarys
import serial, time, io

#global variables
default_prg_file = "ProgDemo.chili"
com_port = "/dev/ttyUSB0"
gs = 15 #GUI spacing
bw = 10 #GUI Button width
motor_state="1" # global motor state
prog_running = 0
prog_pos = 0
last_prog_pos = 0 #to save last program position

#FNC: send command to arduino, wait for errorcode
def send_command(_command):
    arduino.write(_command.encode('utf-8')) #write command, encodes as UTF-8 String
    arduino.flush() #required to get the data out immediately without buffering
    _b = True
    while (_b):
        _errorbyte = arduino.readline()
        if (_errorbyte):
            _errorcode = _errorbyte.decode('utf-8') #convert to string
            _b = False
            l_errorcode.config(text = _errorcode)

#FNC: Wait for data from arduino
def read_data():
    _databyte = arduino.readline()
    _data = _databyte.decode('utf-8').rstrip('\n')
    return _data

#Callback functions for the GUI inputs:
#GUI: Tell robot to drive a single joint in a diraction
def drive_joint(joint, dir):
    command = "10" #command 10 = drive distance
    step = e_step.get() #get step setting from user input field
    #set values for the first 4 joints
    i = 1
    while (i <= 4):
        if (i == joint): command += ","+dir+step
        else: command += ",0"
        i += 1
    #set values for the parallel controlled joints J5 and J6
    if (joint == 5): command += ","+dir+step+","+dir+step
    elif (joint == 6):
        if (dir == "-"):
            command += ",-"+step+","+step
        else:
            command += ","+step+",-"+step
    else: command += ",0,0"
    #set gripper value
    if (joint == 7): command += ","+dir+step
    else: command += ",0"
    #terminate command
    command += "\n"
    #send command
    #msg = messagebox.showinfo("Send Command", "%r" % command)
    send_command(command)

#GUI: Tell robot to zero all joints
def set_home():
    command = "1\n" # command 1 = home
    #send command
    #msg = messagebox.showinfo("Send Command", "%r" % command)
    send_command(command)

#GUI: Tell robot to set motor state to toggled value
def tog_motor_state():
    global motor_state
    #toggle state and set GUI labels
    if (motor_state=="0"):
        l_motor_state.config(bg="green",text="On")
        motor_state="1"
    else:
        l_motor_state.config(bg="red",text="Off")
        motor_state="0"
    #set command
    command = "2,"+motor_state+"\n"
    #send command
    #msg = messagebox.showinfo("Send Command", "%r" % command)
    send_command(command)

#GUI: Run the progamm from the file specified in the GUI
def run_program():
    global prog_running
    global prog_pos
    global last_prog_pos
    prog_running = 1
    l_running.config(bg="green",text = "running...")

#GUI: Pause prgoram execution by resetting motor states and moving programm execution back by one line
def pause_program():
    global prog_running
    global prog_pos
    global last_prog_pos
    prog_running = 0
    prog_pos = last_prog_pos
    l_running.config(bg="red",text = "stopped")
    tog_motor_state()
    tog_motor_state()

#GUI: stop program execution and reset program
def stop_program():
    global prog_running
    global prog_pos
    prog_running = 0
    prog_pos = 0
    l_running.config(bg="red",text = "stopped")
    tog_motor_state()
    tog_motor_state()

#GUI: Save current robot positon to robot programm file as drive_to command
def teach():
    command = "6\n" # command 12 = home
    #send command
    #msg = messagebox.showinfo("Send Command", "%r" % command)
    send_command(command)
    position = read_data()
    # Open file
    fo = open(e_prg_file.get(), "a")
    fo.write( "12," +position + "\n") #write drive command to current position to file
    # Close opened file
    fo.close()

#GUI:send robot new parameter settings for zone
def set_params():
    # command 15 = set speed, command 16 = set zone
    command = "16,"+e_zone.get()+"\n"
    #send command
    #msg = messagebox.showinfo("Send Command", "%r" % command)
    send_command(command)

#GUI:send robot new parameter settings for zone
def run_custom_command():
    command = e_command.get()+"\n"
    #send command
    #msg = messagebox.showinfo("Send Command", "%r" % command)
    send_command(command)

#GUI: open serial connection to arduino
def start_serial():
    global arduino
    #open serial connection
    arduino = serial.Serial(com_port, 9600, timeout=1)
    time.sleep(1) #give the connection a second to settle

#Gererate GUI body
root = Tk()
root.title("C.H.I.L.I.Bot Serial Control")
root.geometry(str(60*gs)+"x"+str(32*gs))

#Generate GUI elements
#GUI:Drive Joints Block1:
l_drive_joints = Label(root, text = "Drive joints"); l_drive_joints.place(x = 1*gs,y = 1*gs)
l_step = Label(root, text = "Step:"); l_step.place(x = 35*gs,y = 1*gs)
e_step = Entry(root, bd = 2, width = 5); e_step.place(x = 37*gs,y = 1*gs); e_step.insert(END, '100')

#GUI:Drive Joints Block3:
f_djt = Frame(root); f_djt.place(x = 1*gs,y = 3*gs)
b_1p = Button(f_djt, width = bw, text = "Z +", command = lambda: drive_joint(1,"-")); b_1p.pack(side = LEFT,padx=2)
b_2p = Button(f_djt, width = bw, text = "Shoulder +", command = lambda: drive_joint(2,"")); b_2p.pack(side = LEFT,padx=2)
b_3p = Button(f_djt, width = bw, text = "Elbow +", command = lambda: drive_joint(3,"")); b_3p.pack(side = LEFT,padx=2)
b_4p = Button(f_djt, width = bw, text = "Hand +", command = lambda: drive_joint(4,"")); b_4p.pack(side = LEFT,padx=2)
b_5p = Button(f_djt, width = bw, text = "J5 +", command = lambda: drive_joint(5,"")); b_5p.pack(side = LEFT,padx=2)
b_6p = Button(f_djt, width = bw, text = "J6 +", command = lambda: drive_joint(6,"")); b_6p.pack(side = LEFT,padx=2)
b_gp = Button(f_djt, width = 12, text = "Gripper open", command = lambda: drive_joint(7,"-")); b_gp.pack(side = LEFT)

#GUI:Drive Joints Block3:
f_djb = Frame(root); f_djb.place(x = 1*gs,y = 5*gs)
b_1m = Button(f_djb, width = bw, text = "Z -", command = lambda: drive_joint(1,"")); b_1m.pack(side = LEFT,padx=2)
b_2m = Button(f_djb, width = bw, text = "Shoulder -", command = lambda: drive_joint(2,"-")); b_2m.pack(side = LEFT,padx=2)
b_3m = Button(f_djb, width = bw, text = "Elbow -", command = lambda: drive_joint(3,"-")); b_3m.pack(side = LEFT,padx=2)
b_4m = Button(f_djb, width = bw, text = "Hand -", command = lambda: drive_joint(4,"-")); b_4m.pack(side = LEFT,padx=2)
b_5m = Button(f_djb, width = bw, text = "J5 -", command = lambda: drive_joint(5,"-")); b_5m.pack(side = LEFT,padx=2)
b_6m = Button(f_djb, width = bw, text = "J6 -", command = lambda: drive_joint(6,"-")); b_6m.pack(side = LEFT,padx=2)
b_gm = Button(f_djb, width = 12, text = "Gripper close", command = lambda: drive_joint(7,"")); b_gm.pack(side = LEFT)

#GUI:Special Commands Block
f_special = Frame(root); f_special.place(x = 1*gs,y = 8*gs)
b_tog_motor_state = Button(f_special, text = "Toggle Motor state", command = tog_motor_state); b_tog_motor_state.pack(side = LEFT,padx=2)
l_motor_state = Label(f_special, width = 3, text = "On", bg = "green"); l_motor_state.pack(side = LEFT, padx=2)
b_set_home = Button(f_special, text = "Teach home", command = set_home); b_set_home.pack(side = LEFT, padx=2)

#GUI:Programming Block
l_programming = Label(root, text = "Programming"); l_programming.place(x = 1*gs,y = 11*gs)
f_programming = Frame(root); f_programming.place(x = 1*gs,y = 13*gs)
b_run = Button(f_programming, width = bw, text = "RUN", command = run_program); b_run.pack(side = LEFT, padx=2)
b_pause = Button(f_programming, width = bw, text = "PAUSE", command = pause_program); b_pause.pack(side = LEFT, padx=2)
b_stop = Button(f_programming, width = bw, text = "STOP", command = stop_program); b_stop.pack(side = LEFT, padx=2)
l_running = Label(f_programming, width = bw, text = "stopped", bg = "red"); l_running.pack(side = LEFT, padx=2)
b_teach = Button(f_programming, width = bw, text = "Teach Position", command = teach); b_teach.pack(side = LEFT, padx=2)
l_prg_file = Label(f_programming, text = "File:"); l_prg_file.pack(side = LEFT, padx=2)
e_prg_file = Entry(f_programming, bd = 2, width = 2*bw); e_prg_file.insert(END, default_prg_file); e_prg_file.pack(side = LEFT, padx=2)
l_current_com = Label(root, text = "", width = 4*bw+6, bg="white"); l_current_com.place(x = 1*gs+2,y = 15*gs)

#GUI:Paramater Block
b_set_params = Button(root, text = "Set Paramaters", command = set_params); b_set_params.place(x = 1*gs,y = 17*gs)
f_params = Frame(root); f_params.place(x = 1*gs,y = 19*gs)
l_zone = Label(f_params, text = "Zone:"); l_zone.pack(side = LEFT, padx=2)
e_zone = Entry(f_params, bd = 2, width = 5); e_zone.insert(END, '5'); e_zone.pack(side = LEFT, padx=2)
l_speed = Label(f_params, text = "Speed:"); l_speed.pack(side = LEFT, padx=2)
e_speed = Entry(f_params, bd = 2, width = 5); e_speed.insert(END, '255'); e_speed.pack(side = LEFT, padx=2)

#GUI:Debug Block
l_command = Label(root, text = "Custom Commands"); l_command.place(x = 1*gs,y = 22*gs)
f_debug = Frame(root); f_debug.place(x = 1*gs,y = 24*gs)
e_command = Entry(f_debug, bd = 2, width = 4*bw); e_command.insert(END, '0,0,0,0,0,0,0,0,0'); e_command.pack(side = LEFT)
b_run_command = Button(f_debug, text = "Run Command", command = run_custom_command); b_run_command.pack(side = LEFT)
l_errorcode = Label(root, text = "", width = 4*bw+6, bg="white"); l_errorcode.place(x = 1*gs,y = 26*gs)

#Serial
b_serial = Button(root, text="Start Serial", command = start_serial);b_serial.place(x = 1*gs,y = 29 * gs)

#root.mainloop()
while True:
    root.update()
    if prog_running:
        _prog_pos = prog_pos #make it so nothing can change this during execution
        last_prog_pos = _prog_pos #save last program position
        #read next command from file
        fo = open(e_prg_file.get(), "r")
        fo.seek(_prog_pos, 0) #go to current reading position
        command = fo.readline()
        prog_pos = fo.tell() # Check current position
        fo.close() # Close opened file
        #check if end of file was reached
        if (command == ""):
            prog_pos = 0
        else:
            #send read command from file to robot
            msg = messagebox.showinfo("Send Command", "%r" % command)
            send_command(command)
            #wait for ready message (polling)
            b = 0
            while b==0:
                send_command("20\n")
                b = read_data()#
                root.update()
