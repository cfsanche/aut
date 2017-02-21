#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
// IoT platform Credentials
String apiKey = "NVP8WNAML4DDIXTM";
const char* logServer = "api.thingspeak.com";

// Internet router credentials
const char* ssid = "Camila’s iPhone (2)";
const char* password = "F@milia7";

ESP8266WebServer server(80); //always included 

void setup() {
Serial.begin(115200); //changed 
WiFi.mode(WIFI_AP_STA);
Serial.println("Starting setup");
setupAccessPoint();
}
// Handling the / root web page from my server
void handle_index() {
  server.send(200, "text/plain", "Get the f**k out from my server!");
}

// Handling the /feed page from my server
void handle_feed() {
String t = server.arg("button");
//String h = server.arg("hum");

server.send(200, "text/plain", "This is response to client");
setupStMode(t);
}

void setupAccessPoint(){
Serial.println("** SETUP ACCESS POINT **");
Serial.println("- disconnect from any other modes");
WiFi.disconnect();
Serial.println("- start ap with SID: "+ String(ssid));
WiFi.softAP(ssid, password);
IPAddress myIP = WiFi.softAPIP();
Serial.println("- AP IP address is :");
Serial.print(myIP);
setupServer();
}

void setupServer(){
Serial.println("** SETUP SERVER **");
Serial.println("- starting server :");
server.on("/", handle_index);
server.on("/feed", handle_feed);
server.begin();
}

void setupStMode(String t){
Serial.println("** SETUP STATION MODE **");
Serial.println("- disconnect from any other modes");
WiFi.disconnect();
Serial.println();
Serial.println("- connecting to Home Router SSID");
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println();
Serial.println("- succesfully connected");
Serial.println("- starting client");

WiFiClient client;

Serial.println("- connecting to Database server: " + String(logServer));
  if (client.connect(logServer, 80)) {
Serial.println("- succesfully connected");
String postStr;
postStr = "&field1=";
postStr += String(t);
//postStr += "&field2=";
//postStr += String(v);
postStr += "\r\n\r\n";
Serial.println("- sending data...");
   client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);
}
client.stop();
Serial.println("- stopping the client");
/** If your ESP does not respond you can just
  *** reset after each request sending 
  Serial.println("- trying reset");
  ESP.reset();
  **/
ESP.reset();
}

void loop() {
  server.handleClient();
}
