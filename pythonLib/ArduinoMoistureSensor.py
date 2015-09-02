import smbus

class ArduinoMoistureSensor:

	def __init__(self, bus, adress):
		self.bus = smbus.SMBus(bus)
		self.busAdress = adress
		self.sensors = [0,0,0,0]

		self.read()

	def read(self):
		data = self.bus.read_i2c_block_data(self.busAdress,0)

		for i in range(0,3):
			self.sensors[i] = (data[2*i] << 8)+ data[2*i+1]

