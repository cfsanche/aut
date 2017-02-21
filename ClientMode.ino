#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// AP Wi-Fi credentials
const char* ssid = "DMENET";
const char* password = "LibDmE2016";

int val =0;

// Local ESP web-server address
String serverHost = "http://192.168.4.1/feed";
String data;
// DEEP_SLEEP Timeout interval
int sleepInterval = 1;
// DEEP_SLEEP Timeout interval when connecting to AP fails
int failConnectRetryInterval = 2;
int counter = 0;

float h;
float t=2.00;
// Static network configuration
IPAddress ip(192, 168, 4, 4);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

WiFiClient client;

void setup() {
pinMode(2, INPUT);
ESP.eraseConfig();
WiFi.persistent(false);
Serial.begin(115200);
Serial.println();
Serial.println("**************************");
Serial.println("**************************");
Serial.println("******** BEGIN ***********");
Serial.println("- start Button sensor");
//delay(500);
Serial.println("- set ESP STA mode");
WiFi.mode(WIFI_STA);
Serial.println("- connecting to wifi");
WiFi.config(ip, gateway, subnet); 
WiFi.begin(ssid, password);
Serial.println("");
while (WiFi.status() != WL_CONNECTED) {
if(counter > 20){
Serial.println("- can't connect, going to sleep");
hibernate(failConnectRetryInterval);
}
delay(500);
Serial.print(".");
counter++;
}

Serial.println("- wifi connected");
Serial.println("Panic Button Sending Ping");
//readDHTSensor();
Serial.println("- build DATA stream string");
buildDataStream();
Serial.println("- send GET request");
sendHttpRequest();
Serial.println();
Serial.println("- go back to sleep");
Serial.println("**************************");
Serial.println("**************************");
//hibernate(sleepInterval);
ESP.deepSleep(0);
}

void sendHttpRequest() {
HTTPClient http;
http.begin(serverHost);
http.addHeader("Content-Type", "application/json"); //json
http.POST(data);
http.writeToStream(&Serial);
http.end();
}

void readDHTSensor() {
delay(200);

//h = dht.readHumidity();
/*val = digitalRead(2);
if(val==0) {
  t = 1.00;
}
else {
  t = 0.00;
}*/
t = 2.00;
//t = dht.readTemperature();
/*if (isnan(h) || isnan(t)) {
    t = 0.00;
    h = 0.00;
  }*/
Serial.println("- temperature read : "+String(t));
//Serial.println("- humidity read : "+String(h));
}

void buildDataStream() {
data = "button=";
data += String(t);
//data += "&hum=";
//data += String(h);
Serial.println("- data stream: "+data);
}

void hibernate(int pInterval) {
WiFi.disconnect();
ESP.deepSleep(10 * 600000 * pInterval, WAKE_RFCAL);
delay(100);
}
void loop() {}
