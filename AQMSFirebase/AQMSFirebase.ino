#include <FirebaseArduino.h>
#include "dht.h"

#include  <ESP8266WiFi.h>

#define FIREBASE_HOST "#########.firebaseio.com"
#define FIREBASE_AUTH "Enter secret code here"
#define WIFI_SSID ""
#define WIFI_PASSWORD ""

#define DHTPIN 14    // Data Pin of DHT 11 , for NodeMCU D5 GPIO no. is 14

#define DHT11_PIN 5   // DHT 11
dht DHT;

void setup() {
  Serial.begin(115200);

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
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}

void loop() {
  int chk = DHT.read11(DHT11_PIN);
  float h = DHT.humidity;
  float t = DHT.temperature;  // Reading temperature as Celsius (the default)

  Serial.print("Humidity: ");  Serial.print(h);
  String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  String fireTemp = String(t) + String("°C");   
 
  Firebase.setFloat("Host1/Humidity", h);
  if (Firebase.failed()) {
      Serial.print("setting /humidity failed:");
      Serial.println(Firebase.error());  
      return;
  }
  //delay(1000);
  Firebase.setFloat("Host1/Temperature", t);
  if (Firebase.failed()) {
      Serial.print("setting /temperature failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);

  DHT.temperature = 0;
  DHT.humidity = 0;

  // remove value
  //Firebase.remove("number");
  //delay(1000);
  
  //Firebase.pushString("/DHT11/Humidity", fireHumid);                                  //setup path and send readings
  //Firebase.pushString("/DHT11/Temperature", fireTemp);                                //setup path and send readings
}
