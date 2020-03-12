// iot water level monitoring

#include <SoftwareSerial.h>
SoftwareSerial nodemcu(11, 10);

#define trigpin 7 // digital pin 7 
#define echopin 6 // digital pin 6

int wpump = 13; // water pump
int flag = 0;
int led1 = A0;
int led2 = A1;
int led3 = A2;
int led4 = A3;
int led5 = A4;
int duration, distance;

int firstVal;
String myString; // complete message from Arduino, which consistors of snesors data
char rdata; // received charactors
String cdata; // complete data


void setup()
{
  Serial.begin(9600);
  nodemcu.begin(9600);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(wpump, OUTPUT);
  digitalWrite(wpump, LOW);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);

  delay(1000);
}

void loop()
{ if (nodemcu.available() == 0 )
  {
    ultrasonic();
  }
  if ( nodemcu.available() > 0 )
  {
    rdata = nodemcu.read();
    myString = myString + rdata;
    //Serial.print(rdata);
    if ( rdata == '\n')
    {
      Serial.println(myString);
      // new code
      String l = getValue(myString, ',', 0);
      firstVal = l.toInt(); //
      myString = "";
      if ( (firstVal == 10) && ( flag == 0) ) // turn off pump
      {
        digitalWrite(wpump, LOW);
        flag = 1;
      }
      if ( (firstVal == 11) && ( flag == 1) ) // turn ON pump
      {
        digitalWrite(wpump, HIGH);
        flag = 0;
      }
    }
  }
}

void ultrasonic()
{ digitalWrite(trigpin, HIGH);
  delayMicroseconds(1000);
  digitalWrite(trigpin, LOW);

  duration = pulseIn(echopin, HIGH);

  distance = ( duration / 2) / 29.1;
  distance = distance - 4;
  Serial.println("cm:");
  nodemcu.println(distance); // send to nodemcu module

  if (  (distance > 0) && (distance <= 10)   )
  {
    digitalWrite(led1, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else if (  (distance > 10) && (distance <= 20)  )
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else if (  (distance > 20) && (distance <= 30)  )
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else if (  (distance > 30) && (distance <= 40)  )
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  } else if (  (distance > 50) && (distance <= 60)  )
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, HIGH);
  } else if (  distance > 60 )
  {
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
  }
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
