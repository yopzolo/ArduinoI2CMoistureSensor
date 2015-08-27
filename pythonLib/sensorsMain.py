import ArduinoMoistureSensor

can1 = ArduinoMoistureSensor(4)
can2 = ArduinoMoistureSensor(8)

can1.read()
can2.read()

for i in range (0,6):
	print can1.sensors[i]
for i in range (0,6):
	print can2.sensors[i]
