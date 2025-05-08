/*
No copyright copyright, free to use, distribute and modify. Provided as-is without warranty. Use at your own risk. Author assumes no liability. 
Intended to be used to monitor the temperature and humidity through a web interface any other use is discouraged and not condoned by the author.
This may cause interference to local wifi networks

NOTE: please change the SSID and PSK before uploading
*/


#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <DHT11.h>


#ifndef APSSID
// UPDATE THIS TO BE YOUR SSID (aka wifi name) AND PASSWORD
#define APSSID "TEMP-HUM-MONITOR"
#define APPSK  "much-science"
#endif

/* Set these to your desired credentials. */
const char *ssid = APSSID;
const char *password = APPSK;

// set the dht sensor data pin to be D4 which we will read from in the handleData and handleRoot functions
DHT11 dht11(2);


ESP8266WebServer server(80);

/* Just a little test message.  Go to http://192.168.4.1 in a web browser
   connected to this access point to see it.
*/
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
      server.send(200, "text/plain", String("{\"celsius\":\""+String(temperature_c)+",\"fahrenheit\":\""+String(temperature_f)+"\",\"humidity\":\""+String(humidity)+"\"}")); // for JSON style data posting
    } else {
// Print error message based on the error code into a json string.
    server.send(200, "text/html", String("{\"error\":"+String(DHT11::getErrorString(result))+"}"));// JSON error
    }
}

void handleNotFound(){
  server.send(404, "text/plain", "404"); // Send HTTP status 404 (Not Found) when there's no handler for the URI in the request
}

void setup() {
  dht11.setDelay(100); // Set this to the desired delay. Default is 500ms. for DHT11 sensor


  delay(1000);
  Serial.begin(115200);
  Serial.println();
  // display some info when the device is being powered on via the serial console
  Serial.print("Configuring access point...");
  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid,password);

  // Default IP address is: 192.168.4.1
  IPAddress APIP = WiFi.softAPIP();
  Serial.print("Temperature/Humidity Sensor IP address: ");
  Serial.print("AP SSID: "+String(APSSID)+"\nAP PASS: "+String(APPSK));
  Serial.println(APIP);
  server.on("/", handleRoot);
  server.on("/api/data", handleJSON);
  server.begin();
  Serial.println("HTTP server is ready for requests");
}

void loop() {
  server.handleClient();
}
