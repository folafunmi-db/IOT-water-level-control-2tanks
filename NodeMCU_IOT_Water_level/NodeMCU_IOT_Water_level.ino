#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#include <SimpleTimer.h>

int pinValue1;

String v2Arduino; // values to Arduino

char auth[] = "insert auth token";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Spectranet-LTE3055";
char pass[] = "dontbeunfortunate";

SimpleTimer timer;

String myString; // complete message from Arduino, which consistors of snesors data
char rdata; // received charactors

int firstVal;
// This function sends Arduino's up time every second to Virtual Pin (1).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V1, millis() / 1000);
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000L, sensorvalue1);
}

void loop()
{
  if (Serial.available() == 0 )
  {
    Blynk.run();
    timer.run(); // Initiates BlynkTimer
    toArduino();
  }
  if (Serial.available() > 0 )
  {
    rdata = Serial.read();
    myString = myString + rdata;
    // Serial.print(rdata);
    if ( rdata == '\n')
    {
      // new code
      String l = getValue(myString, ',', 0);
      firstVal = l.toInt();
      myString = "";
      // end new code
    }
  }
}

void sensorvalue1()
{
  int sdata = firstVal;
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V2, sdata);
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = { 0, -1 };
  int maxIndex = data.length() - 1;

  for (int i = 0; i <= maxIndex && found <= index; i++)
  {
    if (data.charAt(i) == separator || i == maxIndex)
    {
      found++;
      strIndex[0] = strIndex[1] + 1;
      strIndex[1] = (i == maxIndex) ? i + 1 : i;
    }
  }
  return found > index ? data.substring(strIndex[0], strIndex[1]) : "";
}

// in Blynk app writes values to the Virtual Pin V3
BLYNK_WRITE(V10)
{
  pinValue1 = param.asInt(); // assigning incoming value from pin V10 to a variable
  if ( pinValue1 > 60 )
  {
    pinValue1 = 60;
  }
}

void toArduino()
{
  v2Arduino = v2Arduino + pinValue1 + ",";
  Serial.println(v2Arduino);
  delay(100);
  v2Arduino = "";
}
