#include <Arduino.h>
#include "confidentials.h"
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "WiFi.h"

//pins
const int buttonPin = 2;
const int ledPin = 16;

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC   "esp32/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "esp32/sub"

WiFiClientSecure net = WiFiClientSecure();
//MQTTClient client = MQTTClient(256);
PubSubClient client(net);
void messageHandler(char* topic, byte* payload, unsigned int length){
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
  digitalWrite(ledPin, 0);
  delay(1000);
  digitalWrite(ledPin, 1);
}

void connectAWS()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to Wi-Fi");

  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }

  // Configure WiFiClientSecure to use the AWS IoT device credentials
  net.setCACert(AWS_CERT_CA);
  net.setCertificate(AWS_CERT_CRT);
  net.setPrivateKey(AWS_CERT_PRIVATE);

  // Connect to the MQTT broker on the AWS endpoint we defined earlier
  //client.begin(AWS_IOT_ENDPOINT, 8883, net);
  client.setServer(AWS_IOT_ENDPOINT, 8883);
  // Create a message handler
  //client.onMessage(messageHandler);
  client.setCallback(messageHandler);
  Serial.print("Connecting to AWS IOT");

  while (!client.connect(THINGNAME)) {
    Serial.print(".");
    delay(100);
  }

  if(!client.connected()){
    Serial.println("AWS IoT Timeout!");
    return;
  }

  // Subscribe to a topic
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  Serial.println("AWS IoT Connected!");
}

void publishMessage(std::string message)
{
  StaticJsonDocument<200> doc;
  doc["content"] = message;
  char jsonBuffer[512];
  serializeJson(doc, jsonBuffer); // print to client
  client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
}

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT);
  pinMode(ledPin, OUTPUT);
  connectAWS();
}

void loop() {
  int buttonState = digitalRead(buttonPin);
  client.loop();
  if (buttonState == HIGH) {
    Serial.println("btn");
    publishMessage("button pressed");
    delay(1000);
  }
}
