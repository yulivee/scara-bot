#GUI Library
from tkinter import *
from tkinter import messagebox

#Serial Librarys
import serial, time, io

#function to communicate with arduino
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
    elif (joint == 6): command += ","+dir+step+","+dir+step
    else: command += ",0,0"
    #set gripper value
    if (joint == 7): command += ","+dir+step
    else: command += ",0"
    #terminate command
    command += "\n"
    #send command
    msg = messagebox.showinfo("Send Command", "%r" % command)
	send_command(command)

#GUI: Tell robot to zero all joints
def set_home():
    command = "12\n" # command 12 = home
    #send command
    msg = messagebox.showinfo("Send Command", "%r" % command)
    send_command(command)


#GUI: Tell robot to set motor state to value
motor_state="1" # global state
def tog_motor_state():
    global motor_state
    if (motor_state=="0"):
        l_motor_state.config(bg="green",text="On")
        motor_state="1"
    else:
        l_motor_state.config(bg="red",text="Off")
        motor_state="0"
    #set command
    command = "2,"+motor_state+"\n"
    #send command
    msg = messagebox.showinfo("Send Command", "%r" % command)
	send_command(command)

def run_program():
    msg = messagebox.showinfo("Send Command", "run")

def pause_program():
    msg = messagebox.showinfo("Send Command", "pause")

def stop_program():
    msg = messagebox.showinfo("Send Command", "stop")
	
def teach():
    msg = messagebox.showinfo("Send Command", "teach")

#GUI:send robot new parameter settings for zone
def set_params():
    # command 15 = set speed, command 16 = set zone
    command = "16,"+e_zone.get()+"\n"
    #send command
    msg = messagebox.showinfo("Send Command", "%r" % command)
	send_command(command)


#GUI:send robot new parameter settings for zone
def run_custom_command():
    command = e_command.get()+"\n"
    #send command
    msg = messagebox.showinfo("Send Command", "%r" % command)
	send_command(command)


#Gererate GUI body
gs=15 #GUI spacing
bw=10 #Button width

root = Tk()
root.title("C.H.I.L.I. Bot Serial Control")
root.geometry(str(42*gs)+"x"+str(32*gs))

#first GUI element with calming message to user while everything starts up
l_drive_joints = Label(root, text = "Starting, please wait..."); l_drive_joints.place(x = 1*gs,y = 1*gs)

#open serial connection
arduino = serial.Serial('COM10', 115200, timeout=.1)
time.sleep(1) #give the connection a second to settle

#Generate GUI elements
#GUI:Drive Joints Block1:
l_drive_joints.config(text="Drive joints")
l_step = Label(root, text = "Step:"); l_step.place(x = 35*gs,y = 1*gs)
e_step = Entry(root, bd = 2, width = 5); e_step.place(x = 37*gs,y = 1*gs); e_step.insert(END, '100')

#GUI:Drive Joints Block3:
f_djt = Frame(root); f_djt.place(x = 1*gs,y = 3*gs)
b_1p = Button(f_djt, width = bw, text = "Z +", command = lambda: drive_joint(1,"")); b_1p.pack(side = LEFT,padx=2)
b_2p = Button(f_djt, width = bw, text = "Shoulder +", command = lambda: drive_joint(2,"")); b_2p.pack(side = LEFT,padx=2)
b_3p = Button(f_djt, width = bw, text = "Elbow +", command = lambda: drive_joint(3,"")); b_3p.pack(side = LEFT,padx=2)
b_4p = Button(f_djt, width = bw, text = "Hand +", command = lambda: drive_joint(4,"")); b_4p.pack(side = LEFT,padx=2)
b_5p = Button(f_djt, width = bw, text = "J5 +", command = lambda: drive_joint(5,"")); b_5p.pack(side = LEFT,padx=2)
b_6p = Button(f_djt, width = bw, text = "J6 +", command = lambda: drive_joint(6,"")); b_6p.pack(side = LEFT,padx=2)
b_gp = Button(f_djt, width = 12, text = "Gripper open", command = lambda: drive_joint(7,"")); b_gp.pack(side = LEFT)

#GUI:Drive Joints Block3:
f_djb = Frame(root); f_djb.place(x = 1*gs,y = 5*gs)
b_1m = Button(f_djb, width = bw, text = "Z -", command = lambda: drive_joint(1,"-")); b_1m.pack(side = LEFT,padx=2)
b_2m = Button(f_djb, width = bw, text = "Shoulder -", command = lambda: drive_joint(2,"-")); b_2m.pack(side = LEFT,padx=2)
b_3m = Button(f_djb, width = bw, text = "Elbow -", command = lambda: drive_joint(3,"-")); b_3m.pack(side = LEFT,padx=2)
b_4m = Button(f_djb, width = bw, text = "Hand -", command = lambda: drive_joint(4,"-")); b_4m.pack(side = LEFT,padx=2)
b_5m = Button(f_djb, width = bw, text = "J5 -", command = lambda: drive_joint(5,"-")); b_5m.pack(side = LEFT,padx=2)
b_6m = Button(f_djb, width = bw, text = "J6 -", command = lambda: drive_joint(6,"-")); b_6m.pack(side = LEFT,padx=2)
b_gm = Button(f_djb, width = 12, text = "Gripper close", command = lambda: drive_joint(7,"-")); b_gm.pack(side = LEFT)

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
e_prg_file = Entry(f_programming, bd = 2, width = 2*bw); e_prg_file.insert(END, '\\robot_program.chili'); e_prg_file.pack(side = LEFT, padx=2)
l_current_com = Label(root, text = "", width = 4*bw+6, bg="white"); l_current_com.place(x = 1*gs+2,y = 15*gs)

#GUI:Paramater Block
b_set_params = Button(root, text = "Set Paramaters", command = set_params); b_set_params.place(x = 1*gs,y = 17*gs)
f_params = Frame(root); f_params.place(x = 1*gs,y = 19*gs)
l_zone = Label(f_params, text = "Zone:"); l_zone.pack(side = LEFT, padx=2)
e_zone = Entry(f_params, bd = 2, width = 5); e_zone.insert(END, '100'); e_zone.pack(side = LEFT, padx=2)
#l_speed = Label(f_params, text = "Speed(%):"); l_speed.pack(side = LEFT, padx=2)
#e_speed = Entry(f_params, bd = 2, width = 5); e_speed.insert(END, '100'); e_speed.pack(side = LEFT, padx=2)

#GUI:Debug Block
l_command = Label(root, text = "Custom Commands"); l_command.place(x = 1*gs,y = 22*gs)
e_command = Entry(root, bd = 2, width = 4*bw); e_command.insert(END, '0,0,0,0,0,0,0,0,0'); e_command.place(x = 8*gs,y = 24*gs)
b_run_command = Button(root, text = "Run Command", command = run_custom_command); b_run_command.place(x = 1*gs,y = 24*gs)
l_errorcode = Label(root, text = "", width = 4*bw+6, bg="white"); l_errorcode.place(x = 1*gs,y = 26*gs)

root.mainloop()
