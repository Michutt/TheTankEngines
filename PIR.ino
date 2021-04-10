#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <Wire.h>
#include <SimpleTimer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char * ssid =  "NEVERGONNA";
const char * pass =  "GIVEYOUUP";
const char * malinkaIP = "192.168.1.241";

WiFiClient esp;
PubSubClient client(esp);
char message[20];

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int motionSensorPin = 12;

void setup() {
  pinMode(motionSensorPin, INPUT);
  Serial.begin(115200);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  performWifiConnection();
  delay(3000);
  client.setServer(malinkaIP, 1883);
  performMQTTConnection();
  delay(3000);
}

void loop()
{
  displayWifiInfo();
  displayMovementStats();
  delay(1000);
}

void displayMovementStats()
{
 String movementInfo = checkForMovement();
 printText(movementInfo, 1, 1, 30);
 display.display();
 if (movementInfo == "Movement detected!")
 {
   movementInfo.toCharArray(message, 20);
   client.publish("pirData", message);
 }
}

String checkForMovement()
{
  int motionVal = digitalRead(motionSensorPin);
  if (motionVal == 1) {
    return "Movement detected!";
  } else {
    return "No movement";
  }
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

    String clientName = "PIR_Sensor";
    if(client.connect(clientName.c_str()))
    {
      printText("Client connected", 1, 0, 20);
      client.publish("ohayou", "Ohayou gozaimasu from PIR Sensor");
      printText("Hello message sent", 1, 0, 30);
      display.display();
    }
  }
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

String ipToString(IPAddress ip)
{
  String s="";
  for (int i=0; i<4; i++)
    s += i  ? "." + String(ip[i]) : String(ip[i]);
  return s;
}

void printText(String text, int textSize, int x, int y)
{
  display.setTextSize(textSize);
  display.setTextColor(WHITE);
  display.setCursor(x, y);
  display.setTextSize(textSize);
  display.println(text);
}
