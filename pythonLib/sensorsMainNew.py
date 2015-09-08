from ArduinoMoistureSensor import ArduinoMoistureSensor

cans = [ArduinoMoistureSensor(1,6)]

header = "";
for can in cans:
	header+="\t"+str(can.busAdress)


print header

for i in range(0,len(cans[0].sensors)):
	row = str(i);
	for can in cans:
		row += "\t"+str(can.sensors[i]) 
	print row

print "age " + can.time.strftime('%X') 
print "date " + can.time.strftime('%X %x')
