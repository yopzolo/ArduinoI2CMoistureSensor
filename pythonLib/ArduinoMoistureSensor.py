import smbus

class ArduinoMoistureSensor:
	busAdress
	sensors[6]

	def __init__(self, adress):
		self.bus = smbus.SMBus(1)
		self.busAdress = adress

	def read():
		data = bus.read_i2c_block_data(busAdress,0)

		for i in range (0,6):
			sensors[i] = (data[2*i] << 8)+ data[2*i+1]

