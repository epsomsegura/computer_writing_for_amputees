from interface import interface as i
from device import device as device
import threading, sys
from pynput import keyboard
from flask import Flask, request
from flask_socketio import Namespace, SocketIO, emit

app = Flask(__name__)
socketio = SocketIO(app, async_handler=True)

class keylogger:
    gesture = 'standby'
    def __init__(self):
        print('Keylogger')
        self.t = threading.Thread(target=self.server)
        self.t.start()
        
        # Para testear con device.py
        self.t2 = threading.Thread(target=device)
        self.t2.start()
        
        # Para testear con el teclado
        # with keyboard.Listener(on_press=self.keyPress,on_release=self.keyRelease) as l: l.join()

    def server(self):
        socketio.on_namespace(i('/'))
        socketio.run(app)

    def keyPress(self,key):
        self.gesture='standby'
        if(hasattr(key,'char')):
            if(key.char=="a"):
                self.gesture="left"
            elif(key.char=="s"):
                self.gesture="down"
            elif(key.char=="d"):
                self.gesture="right"
            elif(key.char=="w"):
                self.gesture="up"
            elif(key.char=="c"):
                self.gesture="select"
            elif(key.char=="x"):
                self.gesture="erase"
            elif(key.char=="p"):
                sys.exit(0)
            else:
                self.gesture='standby'
        
        f = open("gesture.txt", "w")
        f.write(self.gesture)
        f.close()
        print(self.gesture)
    
    def keyRelease(self,key):
        self.gesture='standby'
        f = open("gesture.txt", "w")
        f.write("standby")
        f.close()


if __name__ == '__main__':
    k=keylogger()