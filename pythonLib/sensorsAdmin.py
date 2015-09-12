from ArduinoMoistureSensor import ArduinoMoistureSensor
import sys

def printUsage (error):
        print "Usage : python sensorsAdmin.py <bus> <adress> [-A <newAdress] -p [-P <newPeriod>]"
	print error
	exit(0)


try:
	bus = int(sys.argv[1])
	adress = int(sys.argv[2])
except (IndexError,ValueError):
	printUsage("bus and address ain't optional")

try:
	can = ArduinoMoistureSensor(bus, adress)
except IOError:
	printUsage("No i2c device on bus "+str(bus)+" adress "+str(adress))

other_args_count = 3
while other_args_count < len(sys.argv):
	arg = sys.argv[other_args_count]
		
	if arg == "-A":
		try:
			newAdress = int(sys.argv[other_args_count+1])
		except (IndexError,ValueError):
			printUsage("-A must be followed by an int representing a new adress")
		else:
                        print "Changing adress from "+str(can.busAdress)+" to "+str(newAdress)
                        can.busAdress = newAdress
			other_args_count+=1

	if arg == "-P":
		try:
			newPeriod = int(sys.argv[other_args_count+1])
		except (IndexError,ValueError):
			printUsage("-P must be followed by an int representing a new period")
		else:
			print "Changing period from "+str(can.period)+" to "+str(newPeriod)
			can.period = newPeriod
			other_args_count+=1

	if arg == "-p":
		print "Current period is set to : "+str(can.period)
			
	other_args_count+=1
