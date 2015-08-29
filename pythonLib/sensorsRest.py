from ArduinoMoistureSensor import ArduinoMoistureSensor

import SimpleHTTPServer
import SocketServer
import json

cans = [ArduinoMoistureSensor(1,4),ArduinoMoistureSensor(1,8)]

class ArduinoMoistureJsonGetHandler(SimpleHTTPServer.SimpleHTTPRequestHandler):

        def do_GET(self):
		sensors = {}
		for can in cans: 
			can.read()
			sensors[can.busAdress]=can.sensors

		response = json.dumps(sensors)

		self.send_response(200)
		self.send_header("Content-type", "application/json")
		self.send_header("Content-length", len(response))
		self.end_headers()
		self.wfile.write(response)


PORT = 8000
httpd = SocketServer.TCPServer(("", PORT), ArduinoMoistureJsonGetHandler)

print "serving at port ",PORT
httpd.serve_forever()
