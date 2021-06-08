/*
 * Arduino communication with  a web server example
 */
 
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>


// Data wire is plugged into digital pin 2 on the Arduino
#define ONE_WIRE_BUS 4 //esp8266 konek ke pin 4 (d2)

//deklarasi one wire
OneWire oneWire(ONE_WIRE_BUS);

//deklaras DS18b20
DallasTemperature sensors(&oneWire);


// persiapan variable
const char *ssid = "xr.07x";  
const char *password = "17171717";

int deviceCount = 0;
float temp[4];
 
//Web/Server address to read/write from 
const char *host = "http://192.168.1.134:1702/api/send";   //your IP/web server address

void setup() {
  delay(1000);
  Serial.begin(115200);
  WiFi.mode(WIFI_OFF);        //Prevents reconnection issue (taking too long to connect)
  delay(1000);
  WiFi.mode(WIFI_STA);        //This line hides the viewing of ESP as wifi hotspot
  
  WiFi.begin(ssid, password);     //Connect to your WiFi router
  Serial.println("");
 
  Serial.print("Connecting");
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
  //If connection successful show IP address in serial monitor
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  //IP address assigned to your ESP

  //  Baca Sensor
  sensors.begin();  // Start up the library
  Serial.begin(9600);
  
  // locate devices on the bus
  deviceCount = sensors.getDeviceCount();
}
 
void loop() {
  //  sensor 
  sensors.requestTemperatures();
  
  //Declare object of class HTTPClient
  HTTPClient http;

  //Prepare data
  String kolam,suhu1,suhu2,suhu3,suhu4, ph, postData;

  //  menampilkan sensor temperatur
  for (int i = 0; i < deviceCount; i++){
    temp[i] = sensors.getTempCByIndex(i);

  }
  int klm_value=1;
  int suhu_value1 = (temp[0]);
  int suhu_value2 = (temp[1]);
  int suhu_value3 = (temp[2]);
  int suhu_value4 = (temp[3]);
  int ph_value = random(0, 14);
  kolam = String(klm_value);
  suhu1 = String(suhu_value1);
  suhu2 = String(suhu_value2);
  suhu3 = String(suhu_value3);
  suhu4 = String(suhu_value4);
  ph = String(ph_value);
 
  //prepare request
  postData = "kolam=" + kolam + "&suhu1=" + suhu1 + "&suhu2=" + suhu2 + "&suhu3=" + suhu3 + "&suhu4=" + suhu4 + "&ph=" + ph ;
  http.begin(host);
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  int httpCode = http.POST(postData);
  String payload = http.getString();
 
  Serial.println(httpCode);
  Serial.println(payload);
  http.end();
  delay(10000); //10 detik sekali

  
}
