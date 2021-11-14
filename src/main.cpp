#include <Arduino.h>
#include <Servo.h>

Servo myservo;
int trigpin = 9;
int echopin = 8;
int sayac = 0;
int redDeger, greenDeger, blueDeger = 0;
int redPin = 2;
int greenPin = 3;
int bluePin = 4;

bool kapakDurum = false;
unsigned long eskiZaman = 0;
unsigned long yeniZaman;
long uzaklik = 0;
int maxKapak = 120;

void kapagiAc()
{
  for (int i = 0; i <= maxKapak; i++)
  {
    myservo.write(i);
    delay(10);
  }
}

void kapagiKapat()
{
  for (int i = maxKapak; i >= 0; i--)
  {
    myservo.write(i);
    delay(10);
  }
}
void ledYak(int durum)
{
  switch (durum)
  {
  case 1:
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 255);
    digitalWrite(bluePin, 255);
    break;
  case 2:
    digitalWrite(redPin, 255);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 255);
    break;
  case 3:
    digitalWrite(redPin, 255);
    digitalWrite(greenPin, 255);
    digitalWrite(bluePin, 0);
    break;
  default:
    break;
  }
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  myservo.attach(7);
  Serial.begin(9600);
  myservo.write(0); //kapak kapansın
  //RGB port ayarları
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  ledYak(1);
}
void loop()
{
  yeniZaman = millis();

  digitalWrite(trigpin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(5);
  long sure = pulseIn(echopin, HIGH);
  uzaklik = sure / 29.1 / 2;

  if (yeniZaman - eskiZaman > 1000)
  {
    Serial.print("uzaklık :");
    Serial.println(uzaklik);

    if (uzaklik < 20 && uzaklik > 0)
    {
      sayac++;
      ledYak(2);
    }
    else
    {
      sayac = 0;
      ledYak(1);
    }

    if (sayac == 3 && kapakDurum == false)
    {
      //myservo.write(90); //kapak açılsın
      kapagiAc();
      ledYak(3);
      sayac = 0;
      kapakDurum = true;
    }
    else if (sayac == 3 && kapakDurum == true)
    {
      //myservo.write(0);//kapak kapansın
      kapagiKapat();
      ledYak(1);
      kapakDurum = false;
    }
    Serial.print("sayac : ");
    Serial.println(sayac);
    Serial.print("kapakDurum :");
    Serial.println(kapakDurum);
    Serial.println("---");
    eskiZaman = yeniZaman;
  }
  delay(50);
}
