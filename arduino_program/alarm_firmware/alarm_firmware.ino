
#include "DHT.h"

#define RedLed  11
#define GreenLed 12
#define DHTPIN 4  
#define DHTTYPE DHT11  
#define Buzzer 3
#define ReedRelay 2
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
      void SendStatusOverSerial(void);
  
  
};

Alarm alarm;

Alarm::Alarm()
{
  
  this->isArmed = false;
  this->enableBuzzer(false);
  digitalWrite(RedLed, HIGH);
  digitalWrite(GreenLed, LOW);
  this->MeasureTempAndHumid();
  this->batteryMeasure();
  this->MeasureReedRelayStatus();
   
}


void Alarm::SendStatusOverSerial()
{
  String command = "";
  command += "B";
  command += String(alarm.batteryCapaity);
  command += "T";
  command += String(alarm.temperature);
  command += "H";
  command += String(alarm.humidity);
  command += "R";
  command += String(alarm.relayStatus);
  Serial.print(command);
  }


void Alarm::MeasureReedRelayStatus(void)
{
  
  this->relayStatus = digitalRead(ReedRelay);
}

void Alarm::batteryMeasure(void)
{
  
    this->batteryCapaity = (int)((float)(analogRead(Battery)*(float)10/(float)1024));  //batteri capacity in % (0 - 100%)
}

void Alarm::PrintOutStatus(void)
{

   Serial.print("Battery: ");
   Serial.println(this->batteryCapaity);
   Serial.print("is armed: ");
   Serial.println(this->isArmed);
   Serial.print("Temp: ");
   Serial.println(this->temperature);
   Serial.print("Humid: ");
   Serial.println(this->humidity);
   
  
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
 // delay(400);
      // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(temperature) ) {
      this->temperature = 0;
      this->humidity= 0;
      return;
    }
    else
    {
      
      this->temperature = (int) temperature;
      this->humidity= (int) humidity;

    }
  
}







void setup() {
    // put your setup code here, to run once:
  pinMode(RedLed, OUTPUT);
  pinMode(GreenLed, OUTPUT);
  pinMode(Buzzer, OUTPUT);
  pinMode(ReedRelay, INPUT);
  pinMode(Battery, INPUT);
  TCCR1A = 0;
  TCCR1B = 0;
  TIMSK1 |= (1 << TOIE1);
  attachInterrupt(digitalPinToInterrupt(ReedRelay), HandleRelayStatusChange, FALLING );

  Serial.begin(9600);
  dht.begin();
}


ISR(TIMER1_OVF_vect)
{
  if(alarm.isArmed == true)
  {
    
    digitalWrite(RedLed, !digitalRead(RedLed)) ;
    alarm.enableBuzzer(digitalRead(RedLed));
    Serial.print("ALERT");

    
    }
  
}

void HandleRelayStatusChange()
{
  
   if(alarm.isArmed == true)
   {

    TCCR1B |= (1 << CS12);   //strt the timer
    
   } 
  
  
  
}


void processSerialData(String message)
{

  if(message.startsWith("DISARM"))
  {
    digitalWrite(RedLed, LOW);
    digitalWrite(GreenLed, HIGH);
    alarm.isArmed = false;
    alarm.enableBuzzer(false);
    TCCR1B &= ~(1 << CS12);  //stop the timer
    Serial.print("DISARM_OK");
  }
  
  if(message.startsWith("ARM"))
  {

    digitalWrite(RedLed, HIGH);
    digitalWrite(GreenLed, LOW);
    alarm.isArmed = true;
    Serial.print("ARM_OK");
  }


  if(message.startsWith("STATUS"))
  {
     alarm.batteryMeasure();
     alarm.MeasureTempAndHumid();
     alarm.MeasureReedRelayStatus();
     alarm.SendStatusOverSerial();
    
  }

    if(message.startsWith("DEBUG"))
  {
    alarm.batteryMeasure();
     alarm.PrintOutStatus();

    
  }


  return;
}

String gettedString;

void loop() {

  while(Serial.available()) {
  
  gettedString= Serial.readString();// read the incoming data as string
  
  }

if(gettedString != NULL)
{
  processSerialData(gettedString);
  gettedString = "";
}



}
