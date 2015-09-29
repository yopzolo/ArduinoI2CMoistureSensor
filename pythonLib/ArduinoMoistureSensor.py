import smbus
import struct
from datetime import datetime, time, timedelta

class ArduinoMoistureSensor (object):

	def __init__(self, bus, adress):
		self.sensorCount = 0
		self.sensors = []
		self.temperature = 0
		self.humidity = 0
		self.time = datetime.now()

		self.bus = smbus.SMBus(bus)
		self.__busAdress = adress

	def read(self):
		data = self.bus.read_i2c_block_data(self.busAdress,0) # TODO change the default register
#		print str(data)+"\n"
		timeOffset = 0
                for i in range(0,4):
                        timeOffset += data[i] << ((3-i)*8)
                self.time = datetime.now() - timedelta(milliseconds=timeOffset)

		self.temperature = ((data[4] << 8) + data[5] + 0.0)/100
		self.humidity = ((data[6] << 8) + data[7] + 0.0)/100

		self.sensorCount = data[8]
		self.sensors = []

		for i in range(0,self.sensorCount):
			self.sensors.append(((data[9+2*i] << 8)+ data[10+2*i] + 0.0)/100)

	@property
	def period(self):
		data = self.bus.read_i2c_block_data(self.busAdress,3)
                period = 0
                for i in range(0,4):
                       	period += data[i] << ((3-i)*8)
		return period

	@period.setter
	def period(self,newPeriod):
		periodInBytes = [0,0,0,0]
		for i in range(0,4):
			periodInBytes[i] = (newPeriod >> ((3-i)*8)) & 0xff

		self.bus.write_i2c_block_data(self.busAdress,2,periodInBytes)

	@property
	def busAdress(self):
		return self.__busAdress

	@busAdress.setter
	def busAdress(self, newBusAdress):
		oldBusAdress = self.__busAdress
		self.__busAdress = newBusAdress

                adressInBytes = [0,0,0,0]
                for i in range(0,4):
                        adressInBytes[i] = (newBusAdress >> ((3-i)*8)) & 0xff

		self.bus.write_i2c_block_data(oldBusAdress,1,adressInBytes)
