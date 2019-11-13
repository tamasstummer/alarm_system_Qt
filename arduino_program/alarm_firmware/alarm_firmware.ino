
#include "DHT.h"


#define RedLed  11
#define GreenLed 12
#define DHTPIN 2  
#define DHTTYPE DHT11  
#define Buzzer 3
#define ReedRelay 10
#define Battery A0


DHT dht(DHTPIN, DHTTYPE);


class Alarm
{

  public:
    int batteryCapaity;
    bool isArmed;
    int temperature;
    int humidity;
    bool relayStatus;
    
  public: 
      Alarm();
      void batteryMeasure(void);
      void enableBuzzer(bool);
      void MeasureTempAndHumid(void);
      void MeasureReedRelayStatus(void);
      void PrintOutStatus(void);
  
  
};

Alarm::Alarm()
{
  
  this->isArmed = false;
  this->MeasureTempAndHumid();
  this->batteryMeasure();
  this->MeasureReedRelayStatus();
  
   
}


void Alarm::MeasureReedRelayStatus(void)
{
  
  this->relayStatus = digitalRead(ReedRelay);
}

void Alarm::batteryMeasure(void)
{
  
 // this->batteryCapaity = (int)((float)(analogRead(Battery)*100/1024));  //batteri capacity in % (0 - 100%)
    this->batteryCapaity = analogRead(Battery);
}

void Alarm::PrintOutStatus(void)
{

   Serial.print("Bat");
//   Serial.print(this->batteryCapaity);
//   Serial.print("armed");
//   Serial.print(this->isArmed);
//   Serial.print("Temp");
//   Serial.print(this->temperature);
//   Serial.print("Humid");
//   Serial.println(this->humidity);
   //Serial.flush();
   
  
}





void Alarm::enableBuzzer(bool signal)
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



void Alarm::MeasureTempAndHumid()
{
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    float humidity = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float temperature = dht.readTemperature();
  
      // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature) ) {
     // Serial.println("Failed to read from DHT sensor!");
      return;
    }
    else
    {
      
      this->temperature = (int) temperature;
      this->humidity= (int) humidity;

    }
  
}



Alarm alarm;



void setup() {
    // put your setup code here, to run once:
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(ReedRelay, INPUT);
  pinMode(Battery, INPUT);
  analogWrite(Buzzer, 128);
  digitalWrite(RedLed, LOW);
  digitalWrite(GreenLed, LOW);
  Serial.begin(9600);
  dht.begin();
}


void loop() {
alarm.PrintOutStatus();
//alarm.batteryMeasure();
//alarm.MeasureTempAndHumid();
//alarm.MeasureReedRelayStatus();





//digitalWrite(RedLed, !digitalRead(RedLed)) ;
//digitalWrite(GreenLed, !digitalRead(GreenLed)) ;
//enableBuzzer(digitalRead(RedLed));

delay(3000);

}
