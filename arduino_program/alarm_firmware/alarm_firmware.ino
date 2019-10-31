
#include "DHT.h"



#define RedLed  11
#define GreenLed 12
#define DHTPIN 2  
#define DHTTYPE DHT11  
#define Buzzer 3


DHT dht(DHTPIN, DHTTYPE);

void enableBuzzer(bool signal)
{
  if(signal == true)
   {
     analogWrite(Buzzer, 128);
    }
  else 
   {
     analogWrite(Buzzer, 0);
    }
  
  }




void setup() {
    // put your setup code here, to run once:
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  analogWrite(Buzzer, 128);
  digitalWrite(RedLed, HIGH);
  digitalWrite(GreenLed, LOW);
  Serial.begin(9600);
  
  dht.begin();
}

void loop() {


  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) ) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }


digitalWrite(RedLed, !digitalRead(RedLed)) ;
digitalWrite(GreenLed, !digitalRead(GreenLed)) ;
enableBuzzer(digitalRead(RedLed));
delay(500);



}
