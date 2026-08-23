import serial
import time

arduino = serial.Serial('COM7', 9600)
time.sleep(2)

answer = "Hello from Python!"

arduino.write((answer + "\n").encode("utf-8"))