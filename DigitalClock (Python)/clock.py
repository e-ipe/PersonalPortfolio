# with packages, this project takes less lines of code
from tkinter import *
from time import strftime

# finding the window the clock will be in
myWindow = Tk()
myWindow.title("My Clock")

# clock functions
def time():
        # format for time
        myTime = strftime('%H:%M:%S %p')
        clock.config(text = myTime)
        clock.after(1000, time)

# formatting for clock
clock = Label(myWindow, font = ('arial', 40, 'bold'), background = 'dark green', foreground = 'white')
clock.pack(anchor = 'center')
time()
mainloop()