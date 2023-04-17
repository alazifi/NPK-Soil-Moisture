#define BLYNK_TEMPLATE_ID "XXXXXXX"
#define BLYNK_DEVICE_NAME "XXXXXXX"
#define BLYNK_FIRMWARE_VERSION  "0.1.0"
#define BLYNK_PRINT Serial
#define APP_DEBUG
#define USE_WEMOS_D1_MINI

#include "BlynkEdgent.h"

unsigned long previousMillis = 0;
unsigned long interval = 5000;

#include <SoftwareSerial.h>
#include <Wire.h>

#define RX D5
#define RO D7
#define DI D2
 
const byte code[]= {0x01, 0x03, 0x00, 0x1e, 0x00, 0x03, 0x65, 0xCD};

byte nilaiNPK[11];
SoftwareSerial mod(RO,DI);// mod(13,16); //(RO, DI)
 
const int lowSM = 744;
const int highSM = 312;

int nit, fos, kal;

struct variabelNPK {byte nitrogen, fosfor, kalium;};

variabelNPK npk;

void setup()
{
  Serial.begin(9600);
  mod.begin(9600);
  pinMode(RX, OUTPUT);
  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
  sensorNPK();
  delay(500);
}

byte moisture(){
  int moistAnalog = analogRead(A0);
  int moistPercent = map(moistAnalog, lowSM, highSM, 0, 100);  
    if (moistPercent >= 100){moistPercent = 100;}
    else if (moistPercent <= 0){moistPercent = 0;}
    else {moistPercent = moistPercent;}
  return moistPercent;
}

void sensorNPK(){
  digitalWrite(RX, HIGH);  
  delay(10);
  if (mod.write(code, sizeof(code)) == 8){
    digitalWrite(RX, LOW);
    for (byte i = 0; i < 11; i++){nilaiNPK[i] = mod.read();}
  }
  nit = nilaiNPK[8];
  fos = nilaiNPK[4];
  kal = nilaiNPK[6];

  npk.nitrogen = nit;
  npk.fosfor = fos;
  npk.kalium = kal;

  Serial.print("Nitrogen: ");Serial.print(npk.nitrogen);Serial.println(" mg/kg");
  Serial.print("Fosfor: ");Serial.print(npk.fosfor);Serial.println(" mg/kg");
  Serial.print("Kalium: ");Serial.print(npk.kalium);Serial.println(" mg/kg");
  Serial.print("Kelelmbaban Tanah: ");Serial.print(moisture());Serial.println("%");
  Serial.println("--------------");
  
  Blynk.virtualWrite(V0, npk.nitrogen);
  Blynk.virtualWrite(V1, npk.fosfor);
  Blynk.virtualWrite(V2, npk.kalium);
  Blynk.virtualWrite(V3, moisture());  
  }
