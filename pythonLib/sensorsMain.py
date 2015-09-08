from ArduinoMoistureSensor import ArduinoMoistureSensor

cans = [ArduinoMoistureSensor(1,4),ArduinoMoistureSensor(1,6),ArduinoMoistureSensor(1,8)]

header = "";
timeHeader = "";
for can in cans:
	header+="\t"+str(can.busAdress)+"\t"
	timeHeader+="\t"+can.time.strftime('%X')


print header
print timeHeader

for i in range(0,len(cans[0].sensors)):
	row = str(i);
	for can in cans:
		row += "\t"+str(can.sensors[i])+"\t" 
	print row

