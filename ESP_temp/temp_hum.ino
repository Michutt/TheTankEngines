#include "DHT.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define DHTPIN 2
#define DHTTYPE DHT11  
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET     -1

const char * ssid =  "UPC246291656";  //NEVERGONNA
const char * pass =  "KXECSTJQ";  //GIVEYOUUP
const char * malinkaIP = "192.168.1.241";

WiFiClient esp;
PubSubClient client(esp);
char message[20];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(D0, OUTPUT);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) 
  { 
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  performWifiConnection();
  delay(3000);
  client.setServer(malinkaIP, 1883);
  performMQTTConnection();
  delay(3000);
}

void loop() {
  display.clearDisplay();
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  if (isnan(h) || isnan(t)) 
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  delay(1000);
  digitalWrite(D0, LOW);
  printText("Humidity: " + String(h), 2, 0, 0);
  printText("Temperature: " + String(t), 2, 0, 30);
  delay(1000);
  display.display();
  digitalWrite(D0, HIGH);
  displayWifiInfo();
  delay(1000);

  String tempData = "temp: " + String(t);
  String humData =  "hum: " + String(h);
  tempData.toCharArray(message, 20);
  client.publish("temperature", message);
  Serial.print(message);
  humData.toCharArray(message, 20);
  client.publish("humidity", message);
  Serial.print(message);

}

void printText(String text, int textSize, int x, int y) 
{
  display.setTextSize(textSize);
  display.setTextColor(WHITE); 
  display.setCursor(x, y);  
  display.setTextSize(1); 
  display.println(text);
}

void performWifiConnection()
{
  printText("Connecting to WiFi", 1, 0, 0);
  display.display();
  delay(1000);
  display.clearDisplay();
  WiFi.begin(ssid, pass);
  int i = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
  delay(500);
  printText(".", 1, i, 0);
  i+=5;
  display.display();
  }
  display.clearDisplay();
  printText("WiFi connected!", 1, 0, 0);
  display.display();
}

void performMQTTConnection()
{
  display.clearDisplay();
  int i = 0;
  while(!client.connected())
  {
    printText("Connecting to MQTT", 1, 0, 0);
    display.display();
    delay(10);
    printText(".", 1, i, 10);
    i+=5;
    display.display();

    String clientName = "DHT_sensor";
    if(client.connect(clientName.c_str()))
    {
      printText("Client connected", 1, 0, 20);
      client.publish("ohayou", "Ohayou gozaimasu from DHT Sensor");
      printText("Hello message sent", 1, 0, 30);
      display.display();
    }
  }
}

String ipToString(IPAddress ip)
{
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void displayWifiInfo()
{
 display.clearDisplay();
 String wifiInfo = ipToString(WiFi.localIP());
 printText("Local network address:", 1, 0, 0);
 display.display();
 printText(wifiInfo, 1, 0, 10);
 display.display();
}
