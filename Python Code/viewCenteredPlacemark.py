#!C:\Program Files\Python35\python

import cgi
import math
from datetime import datetime
import serial
import time

# the following 3 lines will need to be modified to match your setup/ & location
center_lng = -85.198159 #Antenna's GPS Longtitude
center_lat = 35.123637 #Antenna's GPS Latitude
ser = serial.Serial('COM10', 9600, timeout=0)

url = cgi.FieldStorage()
heading = 0.0   
try:
  ser.write(b'OK2Snd\n')
  time.sleep(0.05)
  val1 = ser.readline()
  if val1:
      try:
        heading = float(val1)
      except ValueError:
        message = "Either val1 ({!r}) is not a valid number".format(val1)
        ##print("Error!", message)
        tkMessageBox.showerror("Invalid Inputs", message)  
      ##print( heading)
      ##time.sleep(0.5)
except ser.SerialTimeoutException:
  tkMessageBox.showerror("Data could not be read")


##bbox = url['BBOX'].value
##bbox = bbox.split(',')
##west = float(bbox[0])
##south = float(bbox[1])
##east = float(bbox[2])
##north = float(bbox[3])
##
##center_lng = ((east - west) / 2) + west
##center_lat = ((north - south) / 2) + south


R = 6378.1 #Radius of the Earth
brng = math.radians(heading) #heading is 90 degrees converted to radians.
d = 40              #Distance in km
d = math.pi*R*0.99  #Distance in km


lat1 = math.radians(center_lat) #Current lat point converted to radians
lon1 = math.radians(center_lng) #Current long point converted to radians

lat2 = math.asin( math.sin(lat1)*math.cos(d/R) +
     math.cos(lat1)*math.sin(d/R)*math.cos(brng))

lon2 = lon1 + math.atan2(math.sin(brng)*math.sin(d/R)*math.cos(lat1),
             math.cos(d/R)-math.sin(lat1)*math.sin(lat2))

lat2 = math.degrees(lat2)
lon2 = math.degrees(lon2)


kml = ( 
   '<?xml version="1.0" encoding="UTF-8"?>\n'
   '<kml xmlns="http://www.opengis.net/kml/2.2">\n'
   '<Document>\n'
   '<Style id="Mylinestyle">\n'
   '<LineStyle>\n'
   '<color>7f0000ff</color>\n'
   '<width>4</width>\n'
   '</LineStyle>\n'
   '</Style>\n'
   '<Placemark>\n'
   '<name>Heading: {4:.0f}</name>\n'
   '<styleUrl>#Mylinestyle</styleUrl>\n'
   '<LineString>\n'
   '<extrude>1</extrude>\n'
   '<tessellate>1</tessellate>\n'
   '<coordinates>\n'
   '{0:.14f},{1:.14f},0 {2:.14f},{3:.14f},0\n'
   '</coordinates>\n'
   '</LineString>\n'
   '</Placemark>\n'
   '</Document>\n'
   '</kml>'
   ) .format(center_lng, center_lat, lon2, lat2, heading)

print('Content-Type: application/vnd.google-earth.kml+xml\n')
print(kml)
