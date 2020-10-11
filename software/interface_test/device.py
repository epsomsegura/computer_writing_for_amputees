import serial

port = "/dev/cu.usbserial-1410"
arduino = serial.Serial(port,57600)

class device:
    gesture = 'standby'
    def __init__(self):
        while True:
            line = (arduino.readline()).decode('utf-8').rstrip()
            f = open("gesture.txt", "w")
            f.write(line)
            f.close()
            print(line)