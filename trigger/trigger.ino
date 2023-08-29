#include <WiFiNINA.h>
#include <WiFiUdp.h>
#include <SPI.h>

const char* ssid = "Harshi***";
const char* password = "Sanan***";
const char* resource = "https://maker.ifttt.com/trigger*****";
int keyIndex = 0;
int LED = 3;  // Use pin 3 for LED
int MOTION_SENSOR = 2;  // Use pin 2 for motion sensor
const char* server = "maker.ifttt.com";

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(MOTION_SENSOR, INPUT);
  
  Serial.begin(115200);
  initWifi();
}

void loop() {
  bool motionDetected = digitalRead(MOTION_SENSOR);
  
  if (motionDetected == HIGH) {
    digitalWrite(LED, HIGH);
    makeIFTTTRequest();
    delay(1000);  // Delay to prevent multiple triggers
  } else {
    digitalWrite(LED, LOW);
  }
}

void initWifi() {
  Serial.print("Connecting to: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  int timeout = 10 * 4; 
  while (WiFi.status() != WL_CONNECTED && (timeout-- > 0)) {
    delay(250);
    Serial.print(".");
  }
  Serial.println("");

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Failed to connect");
  }

  Serial.print("WiFi connected, IP address: ");
  Serial.println(WiFi.localIP());
}

void makeIFTTTRequest() {
  Serial.print("Connecting to ");
  Serial.println(server);

  WiFiClient client;
  int retries = 5;
  while (!client.connect(server, 80) && (retries-- > 0)) {
    Serial.print(".");
  }
  Serial.println();

  if (!client.connected()) {
    Serial.println("Failed to connect");
  }

  Serial.print("Request resource: ");
  Serial.println(resource);
  client.print(String("GET ") + resource +
               " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" +
               "Connection: close\r\n\r\n");

  int timeout = 5 * 10;
  while (!client.available() && (timeout-- > 0)) {
    delay(100);
  }

  if (!client.available()) {
    Serial.println("No response");
  }

  while (client.available()) {
    Serial.write(client.read());
  }

  Serial.println("\nclosing connection");
  client.stop();
}
