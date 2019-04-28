#include <FirebaseArduino.h>
#include "dht.h"

#include  <ESP8266WiFi.h>

#define FIREBASE_HOST "XXXXXXX.firebaseio.com"
#define FIREBASE_AUTH "Enter secret code here"


char ssid[] = "SSID1";           // SSID of your home WiFi
char pass[] = "Password1";            // password of your home WiFi
char ssid1[] = "SAM";               // SSID of your home WiFi
char pass1[] = "1234556789";
int timeout=0;
byte ledPin = 2;
int failed;

#define DHT11_PIN 2   // DHT 11
dht DHT;

void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(ssid, pass);
  Serial.print("Connecting to ");Serial.print(ssid);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(ledPin, HIGH);
    Serial.print(".");
    delay(500);
    digitalWrite(ledPin, LOW);
    timeout++;
    if(timeout > 15){
      break;
    }
  }
  if( timeout > 15 ){
     Serial.print("\n\nCouldn't connect to ");
     Serial.println(ssid);
     Serial.print("Connecting to ");
     Serial.print(ssid1);
     WiFi.begin(ssid1, pass1);                 // connects to the SAM
     while (WiFi.status() != WL_CONNECTED) {
       digitalWrite(ledPin, HIGH);
       Serial.print(".");
       delay(500);
       digitalWrite(ledPin, LOW);
     }
  }
  Serial.println("\nConnected to Wi-Fi!");
  Serial.print("Status: "); Serial.println(WiFi.status());    // Network parameters
  Serial.print("IP: ");     Serial.println(WiFi.localIP());
  Serial.print("Subnet: "); Serial.println(WiFi.subnetMask());
  Serial.print("Gateway: "); Serial.println(WiFi.gatewayIP());
  Serial.print("SSID: "); Serial.println(WiFi.SSID());
  Serial.print("Signal: "); Serial.println(WiFi.RSSI());
  pinMode(ledPin, OUTPUT);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  
}

void loop() {
  failed = 0;
  digitalWrite(ledPin, LOW);    // to show the communication only
  int chk = DHT.read11(DHT11_PIN);
  float h = DHT.humidity;
  float t = DHT.temperature;  // Reading temperature as Celsius (the default)

  Serial.print("Humidity: ");  Serial.print(h);
  //String fireHumid = String(h) + String("%");                                         //convert integer humidity to string humidity 
  Serial.print("%  Temperature: ");  Serial.print(t);  Serial.println("°C ");
  //String fireTemp = String(t) + String("°C");   
 
  Firebase.setFloat("Host1/Humidity", h);
  if (Firebase.failed()) {
      failed = 1;
      Serial.print("setting /humidity failed:");
      Serial.println(Firebase.error());  
      return;
  }
  //delay(1000);
  Firebase.setFloat("Host1/Temperature", t);
  if (Firebase.failed()) {
      failed = 1;
      Serial.print("setting /temperature failed:");
      Serial.println(Firebase.error());  
      return;
  }
  if( failed == 0){
    digitalWrite(ledPin, HIGH);
  }
  DHT.temperature = 0;
  DHT.humidity = 0;
  delay(1000);
  // remove value
  //Firebase.remove("number");
  //delay(1000);
  
  //Firebase.pushString("/DHT11/Humidity", fireHumid);                                  //setup path and send readings
  //Firebase.pushString("/DHT11/Temperature", fireTemp);                                //setup path and send readings
}
