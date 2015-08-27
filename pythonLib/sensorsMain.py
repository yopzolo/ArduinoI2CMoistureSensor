from ArduinoMoistureSensor import ArduinoMoistureSensor
import "time"

can1 = ArduinoMoistureSensor(1,4)
can2 = ArduinoMoistureSensor(1,8)


while True:
	can1.read()
	can2.read()
	
	for i in range (0,6):
		print can1.sensors[i]
	for i in range (0,6):
		print can2.sensors[i]

	print "-----"
	time.sleep(1)