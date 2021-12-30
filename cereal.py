from serial import Serial
import time
import sys
#icons = ['thunderstorm', 'rain', 'snow','clear', 'clouds', 'mist']
ser = Serial('COM3', baudrate=115200, timeout=0.1)
time.sleep(0.5)
def modes(cmd): #cmd: string to be encoded
    time.sleep(0.5)
    ser.write(bytes(cmd, 'utf-8'))
    #time.sleep(0.5)

time.sleep(1)
modes(sys.argv[1])
#examples:
#w0-w5      -> Weather. Blink display of 'thunderstorm'
#t00 - t99  -> Temperature. Scroll display '2' and '3'
#p1-p4      -> Air pollution. Still display of '4'