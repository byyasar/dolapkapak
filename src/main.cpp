#include <Arduino.h>
#include <Servo.h>

//------------Pinler ------------------//
int trigpin = 6;
int echopin = 5;
int sayac = 0;
int servoPin = 7;
int redPin = 2;
int greenPin = 3;
int bluePin = 4;
//int butonPin = 8;
int buzzerPin = 9;
// ------------------------------------//
// ------------ Notalar  --------------//
int C = 262; // do                   
int D = 294; // re                  
int E = 330; // mi                  
int F = 349; // fa                   
int G = 392; // sol                 
int A = 440; // la                   
int B = 494; // si                   
int Cc = 523; // do (ince)    
// ------------------------------------//
// ------------ Değişkenler -----------//
bool kapakDurum = false;
//bool butonDurum = false;
unsigned long eskiZaman = 0;
unsigned long yeniZaman;
long uzaklik = 0;
int maxKapak = 120;
Servo myservo;
// ------------------------------------//

//-------------Fonksiyonlar--------------//
void sesCal(int durum)
{
  switch (durum)
  {
  case 1:
    tone(buzzerPin,C); 
    delay(20); 
    noTone(buzzerPin); 
    delay(10);   
    break;
  case 2:
   tone(buzzerPin,D); 
    delay(30); 
    noTone(buzzerPin); 
    delay(10);
    break;
  }
}
void ledYak(int durum)
{
  switch (durum)
  {
  case 1: //kırmızı yak
    digitalWrite(redPin, 255);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 0);
    break;
  case 2: //yeşil yak
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 255);
    digitalWrite(bluePin, 0);
    break;
  case 3: //mavi yak
    digitalWrite(redPin, 0);
    digitalWrite(greenPin, 0);
    digitalWrite(bluePin, 255);
    break;
  }
}
void kapagiAc()
{
  sesCal(2);
  ledYak(3);
  kapakDurum = true;
  for (int i = 0; i <= maxKapak; i++)
  {
    myservo.write(i);
    delay(10);
  }
}
void kapagiKapat()
{
  sesCal(2);
  kapakDurum = false;
  ledYak(1);
  for (int i = maxKapak; i >= 0; i--)
  {
    myservo.write(i);
    delay(10);
  }
}


//-------------------------------------------//
void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  //pinMode(butonPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  myservo.attach(servoPin);
  Serial.begin(9600);
  myservo.write(0); //açılışta kapak kapansın
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

  if (yeniZaman - eskiZaman > 500)
  {
    /*if (digitalRead(butonPin) == HIGH)
    {
      if (kapakDurum)
      {
       // kapagiKapat();
      }
      else
      {
        //kapagiAc();
      }
      Serial.println("Butona basıldı");
    }*/
    

    if (uzaklik <= 25 && uzaklik > 0)
    {
      sayac++;
      ledYak(2);
      sesCal(1);
      if (sayac == 3 && kapakDurum == false)
      {
        kapagiAc();
        sayac = 0;
      }
      else if (sayac == 3 && kapakDurum == true)
      {
        kapagiKapat();
      }
    }
    else
    {
      sayac = 0;
      ledYak(1);
    }
    Serial.print("uzaklık :");
    Serial.println(uzaklik);
    Serial.print("sayac : ");
    Serial.println(sayac);
    Serial.print("kapakDurum :");
    Serial.println(kapakDurum);
    Serial.println("---");
    eskiZaman = yeniZaman;
  }
  delay(50);
}
