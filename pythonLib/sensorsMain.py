from ArduinoMoistureSensor import ArduinoMoistureSensor

cans = [ArduinoMoistureSensor(1,4),ArduinoMoistureSensor(1,8)]

header = "";
for can in cans:
	can.read()
	header+="\t"+str(can.busAdress)


print header

for i in range(0,len(cans[0].sensors)):
	row = str(i);
	for can in cans:
		row += "\t"+str(can.sensors[i]) 
	print row

