#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "webpage.h"
#include "struct.h"

#define LED 2

SendDataStruct sData;
ReturnDataStruct rData;

const char* ssid = "TIM-31852215";
const char* password = "FrAmArTiNoCaMpAnArO";

ESP8266WebServer server(80);

void handleRoot()
{
  String s = MAIN_page;
  server.send(200, "text/html", s);
}

void handleGoto()
{
  String newRA = server.arg("newRA");
  String newDEC = server.arg("newDEC");
  Serial.println(newRA + " - " + newDEC);
  
  sData.newRA = newRA;
  sData.newDEC = newDEC;
  sData.track = "";
  sData.park = "";
  sData.setParkPosition = "";
  SendMessage(sData);
}

void handleTrack()
{
  String track;
  if(track == "true")
  {
    Serial.println("Start tracking");
    track = "false";
  }
  else if(track == "false")
  {
    Serial.println("Stop tracking");
    track = "true";
  }

  sData.newRA = "";
  sData.newDEC = "";
  sData.track = track;
  sData.park = "";
  sData.setParkPosition = "";
  SendMessage(sData);
}

void handlePark()
{
  Serial.println("handlePark");

  sData.newRA = "";
  sData.newDEC = "";
  sData.track = "";
  sData.park = "park";
  sData.setParkPosition = "";
  SendMessage(sData);
}

void handleSetParkPosition()
{
  Serial.println("Set park position");

  sData.newRA = "";
  sData.newDEC = "";
  sData.track = "";
  sData.park = "";
  sData.setParkPosition = "true";
  SendMessage(sData);
}

void SendMessage(SendDataStruct sendDataStruct)
{
  
}

void handleUpdatePage()
{
  rData = AskForData();
  
  String delimiter = String("|");
  String updateString = rData.currentRA + "-" + rData.currentDEC + delimiter + rData.track + delimiter;
  
  if(rData.park == 0)
    updateString += "Parked";
  else if(rData.park == 1)
    updateString += "Unpark";
  else if(rData.park == 2)
    updateString += "Parking..."; 
  else
    updateString += "Unknown park status";
  
  updateString += delimiter;
  updateString += rData.parkRA + "-" + rData.parkDEC + delimiter;
  
  server.send(200, "text/plane", updateString);
}

ReturnDataStruct AskForData()
{
  ReturnDataStruct returnDataStruct;
  returnDataStruct.currentRA = "123";
  returnDataStruct.currentDEC = "234";
  returnDataStruct.track = "tracking";
  returnDataStruct.park = 1;
  returnDataStruct.parkRA = "345";
  returnDataStruct.parkDEC = "234";

  return returnDataStruct;
}

void setup(void)
{
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  Serial.println("");
  pinMode(LED, OUTPUT); 
  
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
 
  server.on("/", handleRoot);
  server.on("/goto", handleGoto);
  server.on("/track", handleTrack);
  server.on("/park", handlePark);
  server.on("/setParkPosition", handleSetParkPosition);
  server.on("/updatePage", handleUpdatePage);

  server.begin();
  Serial.println("HTTP server started");
}

void loop(void)
{
  server.handleClient();
}
