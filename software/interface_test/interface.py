import pyautogui
from flask import Flask, request
from flask_socketio import Namespace, SocketIO, emit


app = Flask(__name__)
# socketio = SocketIO(app, async_handler=True,engineio_logger=True)
socketio = SocketIO(app, async_handler=True)

class interface(Namespace):      
    gesture = 'standby'
    def __init__(self,namespace):
        self.namespace=namespace
        print('Starting socket server')
        
    def on_connect(self):
        self.gesture = open('gesture.txt', "r").read()
        if(self.gesture!='standby'):
            emit('interaction',self.gesture)
        else:
            emit('interaction',self.gesture)
        f = open("gesture.txt", "w")
        f.write('standby')
        f.close()
    
    def on_select(self, data):
        print('Ready to send '+data)
        if data == 'backbackspace':
            pyautogui.press('backspace')
        elif data == 'spaspace':
            pyautogui.press('space')
        elif data == 'ententer':
            pyautogui.press('enter')
        elif data == 'dodot':
            pyautogui.write('. ', interval=0.05)
        elif data == 'cocomma':
            pyautogui.write(', ', interval=0.05)
        else:
            pyautogui.write(data, interval=0.05)


if __name__ == "__main__":
    socketio.on_namespace(interface('/'))
    socketio.run(app)