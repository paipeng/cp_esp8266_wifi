
#include <Arduino.h>
#include "wifi_const.h"

// WIFI_SSID and WIFI_PASSWD are defined in wifi_const.h
//#define WIFI_SSID ""
//#define WIFI_PASSWD ""

#include <ESP8266WiFi.h>


//WIFI连接路由器
void wifi_connect(void) {
  Serial.print("Connecting to ");
  delay(10);

  WiFi.begin(WIFI_SSID, WIFI_PASSWD);

  Serial.println();
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("success!");
  Serial.print("IP Address is: ");
  Serial.println(WiFi.localIP());

  delay(500);
}

void setup(){
  delay(2000);
  Serial.begin(115200);
  wifi_connect();
  
}
  
void loop(){
  const uint16_t port = 80;
  const char * host = "192.168.1.228"; // ip or dns

  Serial.print("Connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;

  if (!client.connect(host, port)) {
      Serial.println("Connection failed.");
      Serial.println("Waiting 5 seconds before retrying...");
      delay(5000);
      return;
  }

  // This will send a request to the server
  //uncomment this line to send an arbitrary string to the server
  //client.print("Send this data to the server");
  //uncomment this line to send a basic document request to the server
  //client.print("GET /cpiot/version HTTP/1.1\n\n");
  client.print(String("GET ") + "/cpiot/version" + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");    

  int maxloops = 0;
  
  //wait for the server's reply to become available
  while (!client.available() && maxloops < 1000)
  {
    maxloops++;
    delay(1); //delay 1 msec
  }
  if (client.available() > 0)
  {
    //read back one line from the server
    while(client.available()) {
      String line = client.readStringUntil('\r');
      Serial.println(line);
    }
  }
  else
  {
    Serial.println("client.available() timed out ");
  }

  Serial.println("Closing connection.");
  client.stop();

  Serial.println("Waiting 5 seconds before restarting...");
  delay(5000);
}
