from ArduinoMoistureSensor import ArduinoMoistureSensor
from datetime import datetime, time, timedelta
import pickle

cans = [ArduinoMoistureSensor(1,4),ArduinoMoistureSensor(1,8)]

with open('./sensors.log', "a") as logFile:

	logFile.write(datetime.now().isoformat())

	for can in cans:
		can.read()

		for sensor in can.sensors:
	                logFile.write("\t"+str(sensor))

	logFile.write("\n")
