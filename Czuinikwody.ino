

int ledPin = 13;  // LED connected to digital pin 13
int inPin = A0;    // pushbutton connected to digital pin 7
int val = 0;      // variable to store the read value
const int buzzer = D1; //buzzer to arduino pin 9
//int sound = S1;


void setup() {
  pinMode(inPin, INPUT);    // sets the digital pin 7 as input
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output
  //pinMode (sound, INPUT);

  
  
}

void loop() {
  val = analogRead(inPin);   // read the input pin
  //dzwiek = analogRead(sound);   // read the input pin

  Serial.println(val);
  //Serial.println(dzwiek);
  delay(100);
  if (val > 200) 
  {
    digitalWrite(LED_BUILTIN, LOW);
    tone(buzzer, 1000, 50);
    }
     if (val < 200) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    //noTone(buzzer);     // Stop sound...
    }


  
}
//TESTY
