from tkinter import *
from tkinter import messagebox

#Gererate GUI body
top = Tk()
top.geometry("150x120")

#Callback functions for the GUI inputs
def ButtonCallBack():
   msg = messagebox.showinfo( "Message:", E1.get())

#Generate GUI elements
L1 = Label(top, text = "Message:")
L1.place(x = 10,y = 10)
E1 = Entry(top, bd = 5)
E1.place(x = 10,y = 40)
B = Button(top, text = "Get Message", command = ButtonCallBack)
B.place(x = 10,y = 80)

top.mainloop()