 

void Line_Notify(String message) ;

#include <ESP8266WiFi.h>

// Config connect WiFi
#define WIFI_SSID "UbiNES"
#define WIFI_PASSWORD "12345678"

// Line config
#define LINE_TOKEN "yrGpbA8G2AuJ4yZDLmNMo70RB8F71XHIHJKMVLwogtr"

#define SW D2

String message = "I'm ESP8266";

void setup() {
  pinMode(SW, INPUT);

  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  Line_Notify(message);
  delay(5000);
}

void Line_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
    return;   
  }

  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  //req += "User-Agent: ESP8266\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + message).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + message;
   Serial.println(req);
  client.print(req);
    
  delay(20);

  // Serial.println("-------------");
  while(client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
    //Serial.println(line);
  }
  // Serial.println("-------------");
}
