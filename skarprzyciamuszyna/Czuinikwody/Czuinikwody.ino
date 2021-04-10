
#include <SPI.h>
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


const char * ssid =  "NEVERGONNA";
const char * pass =  "GIVEYOUUP";
const char * malinkaIP = "192.168.1.241";


WiFiClient esp;
PubSubClient client(esp);
char message[20];



int ledPin = 13;  // LED connected to digital pin 13
int inPin = A0;    // pushbutton connected to digital pin 7
int val = 0;      // variable to store the read value
String str;
const int buzzer = D1; //buzzer to arduino pin 9


void setup() {
  pinMode(inPin, INPUT);    // sets the digital pin 7 as input
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  //pinMode (sound, INPUT);
  performWifiConnection();
  delay(3000);
  client.setServer(malinkaIP, 1883);
  performMQTTConnection();
  delay(3000);
  
  
}

void loop() {
  val = analogRead(inPin);   // read the input pin

  Serial.println(val);
  delay(1000);

  String(val).toCharArray(message, 20);
  client.publish("waterdetector", message);

  if (val > 200) 
  {
    digitalWrite(LED_BUILTIN, LOW);
    tone(buzzer, 1000, 50);
    }
     if (val < 200) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    }


  
}
void performWifiConnection()
{
  WiFi.begin(ssid, pass);
  Serial.println("WiFi.begin");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
      Serial.println("WiFi.status");

  }
}

void performMQTTConnection()
{ 
  while(!client.connected())
  {
      Serial.println("client.connected");

    String clientName = "WODA_Sensor";
    if(client.connect(clientName.c_str()))
    {
        Serial.println("connectet");
                Serial.println(WiFi.localIP());


      client.publish("ohayou", "Ohayou gozaimasu from WODA Sensor");
     }
  }
}
