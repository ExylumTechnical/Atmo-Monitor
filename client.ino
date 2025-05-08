/*
No copyright copyright, free to use, distribute and modify. Provided as-is without warranty. Use at your own risk. Author assumes no liability. 
Intended to be used to monitor the temperature and humidity through a web interface any other use is discouraged and not condoned by the author.
This may interfere with local networks

How to use:
Change the SSID and PSK to be the wifi name and password of your local network


*/
#include <DHT11.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WiFiMulti.h> 
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>   // Include the WebServer library

ESP8266WiFiMulti wifiMulti;     // Create an instance of the ESP8266WiFiMulti class, called 'wifiMulti'

ESP8266WebServer server(80);    // Create a webserver object that listens for HTTP request on port 80
DHT11 dht11(2); // Set pin for DHT11 sesor to be pin D4

void handleRoot();              // function prototypes for HTTP handlers
void handleNotFound();
//String getDHTSensorInfo();

void setup(void){
  Serial.begin(9600);         // Start the Serial communication to send messages to the computer
  dht11.setDelay(500); // Set this to the desired delay. Default is 500ms. for DHT11 sensor
  delay(10);
  Serial.println('\n');

  wifiMulti.addAP("YOUR_SSID", "YOUR_WIFI_PASSWORD");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("YOUR_OTHER_SSID", "YOUR_OTHER_WIFI_PASSWORD");   // for redundancy
  wifiMulti.addAP("ANOTHER_SSID", "ANOTHER_WIFI_PASSWORD");   // add as many as you like

  Serial.println("Connecting ...");
  int i = 0;
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect: scan for Wi-Fi networks, and connect to the strongest of the networks above
    delay(250);
    Serial.print('.');
  }
  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // Tell us what network we're connected to
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // Send the IP address of the ESP8266 to the computer

  if (MDNS.begin("esp8266")) {              // Start the mDNS responder for esp8266.local
    Serial.println("mDNS responder started");
  } else {
    Serial.println("Error setting up MDNS responder!");
  }

  server.on("/", handleRoot);
  server.on("/api/data", handleJSON);
  
  server.onNotFound(handleNotFound);        // When a client requests an unknown URI (i.e. something other than "/"), call function "handleNotFound"

  server.begin();                           // Actually start the server
  Serial.println("HTTP server started");
}

void loop(void){
  server.handleClient();                    // Listen for HTTP requests from clients
}

void handleRoot() {
    String post;
    int temperature_c = 0;
    int humidity = 0;
    int result = dht11.readTemperatureHumidity(temperature_c, humidity);
    int temperature_f = (temperature_c * 1.8) + 32;
    if (result == 0) {
      // if all is well then print out the data in a human readable format
      server.send(200, "text/html", String("<!DOCTYPE html><html><head><script>setInterval(function(){window.location.reload();}, 1000);</script><title>Temp/Humidity Sensor</title></head><body><h1>Sensor Data</h1><p>Temperature C/F: "+String(temperature_c)+"C / "+String(temperature_f)+"F</p><p>Humidity: "+String(humidity)+"</p></body></html>"));
    } else {
        // Print error message based on the error code.
      server.send(200, "text/html", String("<!DOCTYPE html><html><head><script>setInterval(function(){window.location.reload();}, 1000);</script><title>Temp/Humidity Sensor</title></head><body><h1>Sensor Data</h1><p>Sensor Error:"+String(DHT11::getErrorString(result))+" </p></body></html>"));
    }
}

void handleJSON() {
    String post;
    int temperature_c = 0;
    int humidity = 0;
    int result = dht11.readTemperatureHumidity(temperature_c, humidity);
    int temperature_f = (temperature_c * 1.8) + 32;

    if (result == 0) {
// if the data was accuired successfully then print it out in a json string.
      server.send(200, "text/plain", String("{\"celsius\":\""+String(temperature_c)+",\"ferenheit\":\""+String(temperature_f)+"\",\"humidity\":\""+String(humidity)+"\"}")); // for JSON style data posting
    } else {
// Print error message based on the error code into a json string.
    server.send(200, "text/html", String("{\"error\":"+String(DHT11::getErrorString(result))+"}"));// JSON error
    }
}

void handleNotFound(){
  server.send(404, "text/plain", "404: Not found"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

