import sys
from ArduinoMoistureSensor import ArduinoMoistureSensor

can = ArduinoMoistureSensor(int(sys.argv[1]),int(sys.argv[2]))

can.read();

print "date " + can.time.strftime('%X %x')

for sensor in can.sensors:
	print sensor
