import tkinter
import threading
import socket
from os import linesep

class key_manager:

    key_queue = []

    def stop():
        s1.send(b's\r\n')
        print(s1.recv(1024).decode() + linesep)
        #print('stop')

    def __init__(self, processor):
        self.on = False
        self.processor = processor

    def process(self):
        if len(key_manager.key_queue) != 0:
            key_manager.key_queue[-1].processor()

    def ev_press(self, e):
        if self.on == False:
            self.on = True
            key_manager.key_queue.append(self)
            self.process()

    def ev_unpress(self, e):
        if self.on == True:
            self.on = False
            try:
                if key_manager.key_queue.index(self) != len(key_manager.key_queue) - 1:
                    key_manager.key_queue.remove(self)
                    return
                key_manager.key_queue.remove(self)
            except ValueError as e:
                pass
            if len(key_manager.key_queue) == 0:
                key_manager.stop()
            self.process()


def prtw():
    s1.send(b'f\r\n')
    print(s1.recv(1024).decode() + linesep)
    #print('w')
    
def prtd():
    s1.send(b'r\r\n')
    print(s1.recv(1024).decode() + linesep)
    #print('d')
    
def prts():
    s1.send(b'b\r\n')
    print(s1.recv(1024).decode() + linesep)
    #print('s')
    
def prta():
    s1.send(b'l\r\n')
    print(s1.recv(1024).decode() + linesep)
    #print('a')
    
def prtup(e):
    s1.send(b'm\x5A\r\n')
    print(s1.recv(1024).decode() + linesep)
    #print('up')
    
def prtlf(e):
    s1.send(b'm\x1E\r\n')
    print(s1.recv(1024).decode() + linesep)
    #print('lf')
    
def prtrt(e):
    s1.send(b'm\x96\r\n')
    print(s1.recv(1024).decode() + linesep)
    #print('rt')
    
def prtsp(e):
    s1.send(b'd\r\n')
    dist = str(int.from_bytes(s1.recv(1024)[:-2], byteorder = 'little', signed = False))
    print(dist + linesep)
    label1['text'] = "Distance: " + dist + " cm"
    #print('sp')

s1 = socket.socket()

s1.connect(('192.168.4.1', 4444))

wnd = tkinter.Tk()

wk = key_manager(prtw)
dk = key_manager(prtd)
sk = key_manager(prts)
ak = key_manager(prta)

wnd.bind('<KeyPress-w>', wk.ev_press)
wnd.bind('<KeyRelease-w>', wk.ev_unpress)
wnd.bind('<KeyPress-d>', dk.ev_press)
wnd.bind('<KeyRelease-d>', dk.ev_unpress)
wnd.bind('<KeyPress-s>', sk.ev_press)
wnd.bind('<KeyRelease-s>', sk.ev_unpress)
wnd.bind('<KeyPress-a>', ak.ev_press)
wnd.bind('<KeyRelease-a>', ak.ev_unpress)
wnd.bind('<KeyPress-Up>', prtup)
wnd.bind('<KeyPress-Left>', prtlf)
wnd.bind('<KeyPress-Right>', prtrt)
wnd.bind('<KeyPress-space>', prtsp)

label1 = tkinter.Label(wnd, text = 'Distance: ')

label1.pack()

wnd.geometry('600x600')

wnd.mainloop()