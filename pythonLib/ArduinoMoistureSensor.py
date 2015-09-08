import smbus
from datetime import datetime, time, timedelta

class ArduinoMoistureSensor:

	def __init__(self, bus, adress):
		self.bus = smbus.SMBus(bus)
		self.busAdress = adress
		self.sensors = [0,0,0,0]
		self.time = 0

		self.read()

	def read(self):
		data = self.bus.read_i2c_block_data(self.busAdress,0)

		for i in range(0,4):
			self.sensors[i] = (data[2*i] << 8)+ data[2*i+1]

		timeOffset = 0
		for i in range(0,4):
			timeOffset += data[8+i] << ((3-i)*8)
		self.time = datetime.now() - timedelta(milliseconds=timeOffset)
