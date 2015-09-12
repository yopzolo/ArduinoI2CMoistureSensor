import smbus
import struct
from datetime import datetime, time, timedelta

class ArduinoMoistureSensor (object):

	def __init__(self, bus, adress):
		self.sensors = [0,0,0,0]
		self.time = 0

		self.bus = smbus.SMBus(bus)
		self.__busAdress = adress
		#self.period = 0

		self.read()

	def read(self):
		data = self.bus.read_i2c_block_data(self.busAdress,0) # TODO change the default register

		for i in range(0,4):
			self.sensors[i] = (data[2*i] << 8)+ data[2*i+1]

		timeOffset = 0
		for i in range(0,4):
			timeOffset += data[8+i] << ((3-i)*8)
		self.time = datetime.now() - timedelta(milliseconds=timeOffset)

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
