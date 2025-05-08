# Atmo-Monitor
A small Arduino based project for a atomspheric monitor that is accessable via a http server on a local network or from its own access point.

# Atmo-Monitor
A small Arduino based project for a atomspheric monitor that is accessable via a http server on a local network or from its own access point.

What this is: a fun project
What this is not: production ready or to be used commercially

Forks and suggestions are most welcome

## Requirements:
- the DHT11 Temperature and Humidity Sensor
- 3 jumper cables
- The ESP8266MOD development board
- A wireless network to connect the device to if you are using the client.io file if using the ap.ino file then see the section below named sensor server

## Sensor Setup
1. The signal wire for the DHT11 sensor should be connected to the D4 pin on the Arduino ESP8266 board
2. Install the libraries ( I used the DHT11 library from Dr. Rubasha which can be found here: https://github.com/dhrubasaha08/DHT11 )
3. Change the YOUR_SSID and YOUR_WIFI_PASSWORD in the code accordingly
4. Compile and upload
5. Watch the serial output for the ip address and that it successfully connected to the wifi network
6. Go to a web browser and verify the sensor is reachable and updating.

Try breathing on the sensor and refreshing the page to verify it is reading data.

## Sensor Server Setup
- Plug in the DHT sensor pin to D4 and connect it to the onboard power source (see sensor datasheet for exact details) but generally 3.3v is the best idea
- The data can be viewed in a web browser by browsing to the web root like http://192.168.4.1/
- JSON data is also available on the only API endpoint http://192.168.4.1/api/data
- to view AP ssid, password, and starting IP address upon startup the console port will output this when the device is powered on and is set to BAUD 115200, which means that it is recommended that the device
  is enclosed in a case that does not allow for easy access to the console port.

### TODO: 
  - allow for configuration of the SSID and Password through a webpage
  - add compatibility for connecting to a network and sending data to a collection server this may require some additional data like using the MAC or location to uniquely identify itself and there is a lot to deal with for authentication/authorization/validation on the server side, but i think it would be a cool feature to be able to point the sensor to a server on the LAN that saves the data for analysis

