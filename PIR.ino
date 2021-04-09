#include <Adafruit_SSD1306.h>
#include <splash.h>
#include <SPI.h>
#include <Wire.h>
#include <SimpleTimer.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ESP8266WiFi.h>

const char *ssid =  "NEVERGONNA";
const char *pass =  "GIVEYOUUP";

WiFiClient client;

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET     -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int motionSensorPin = 15;

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
 printText(movementInfo, 1, 1, 20);
 display.display();
}

String checkForMovement()
{
  int motionVal = digitalRead(motionSensorPin);
  if (motionVal == 1) {
    return "Wykryto ruch!";
  } else {
    return "Brak ruchu";
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

void displayWifiInfo()
{
 display.clearDisplay();
 String wifiInfo = ipToString(WiFi.localIP());
 printText(wifiInfo, 1, 0, 0);
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
