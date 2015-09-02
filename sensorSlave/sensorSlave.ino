#include <Wire.h>

#define ARRAY_LENGTH(x)  (sizeof(x) / sizeof((x)[0]))

//#define LOG_SERIAL

//TODO 
//
// store read frequency in EEPROM
// write read frequency via I2C
// read frequency via I2C

const int sensorPins[][2] = {
  {
    A0,4}
  ,{
    A1,3}
  ,{
    A2,2}
  ,{
    A3,1}
};

const int sensor_SIZE = ARRAY_LENGTH(sensorPins);
unsigned int A_Values[sensor_SIZE];

const int addressPins[] = {
  6,7,8,9};

void setup()
{  
  int i2cAddress = 0;
  for (int i=0;i<ARRAY_LENGTH(addressPins);i++){
    pinMode(addressPins[i], INPUT_PULLUP);
    if (digitalRead(addressPins[i]) == HIGH){
      i2cAddress += (1<<i);      
    }
  }

  Wire.begin(i2cAddress);

  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

#ifdef LOG_SERIAL
  Serial.begin(9600);
#endif
}

void loop()
{
  for (int i=0;i<sensor_SIZE;i++){

    pinMode(sensorPins[i][0], INPUT_PULLUP);
    pinMode(sensorPins[i][1], OUTPUT);
    digitalWrite(sensorPins[i][1], LOW);
    delay(50);

    A_Values[i] = 1023-analogRead(sensorPins[i][0]); //Attention à corriger ça

    pinMode(sensorPins[i][0], OUTPUT);
    digitalWrite(sensorPins[i][0], LOW);
    digitalWrite(sensorPins[i][1], HIGH);
    delay(50);

    digitalWrite(sensorPins[i][1], LOW);
    digitalWrite(sensorPins[i][0], LOW);
  }

#ifdef LOG_SERIAL
  for (int i=0;i<sensor_SIZE;i++){
    Serial.print(A_Values[i]);    
    Serial.print("|");
  }
  Serial.println();
#endif

  delay(600);
}

void requestEvent()
{
  byte buffer[12];

  for (int i=0;i<sensor_SIZE;i++){
    buffer[2*i] = A_Values[i] >> 8;
    buffer[2*i+1] = A_Values[i] & 0xff;
  }

  Wire.write(buffer, 12);
}


void receiveEvent(int len){

}
