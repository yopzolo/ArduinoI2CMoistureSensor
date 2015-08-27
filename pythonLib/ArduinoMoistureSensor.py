import smbus
import time

bus = smbus.SMBus(1)
address = int(sys.argv[1])

data = bus.read_i2c_block_data(address,0)

for i in range (0,6):
	print (data[2*i] << 8)+ data[2*i+1]
