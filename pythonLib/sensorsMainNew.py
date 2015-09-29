import sys
from ArduinoMoistureSensor import ArduinoMoistureSensor

can = ArduinoMoistureSensor(int(sys.argv[1]),int(sys.argv[2]))

can.read();

print "date\t\t" + can.time.strftime('%X %x')
print "amb temp\t" + str(can.temperature) + "C"
print "amb humit\t" + str(can.humidity) + "%"

i=0
for sensor in can.sensors:
	print "humidity " + str(i) + "\t" + str(sensor) + "%"
	i+=1
