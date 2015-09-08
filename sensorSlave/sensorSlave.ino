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
    A0,4          }
  ,{
    A1,3          }
  ,{
    A2,2          }
  ,{
    A3,1          }
};

const int sensor_SIZE = ARRAY_LENGTH(sensorPins);
unsigned int A_Values[sensor_SIZE];
unsigned long A_values_timestamp;

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

    pinMode(addressPins[i], OUTPUT);
    digitalWrite(addressPins[i], LOW);
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
  }

  A_values_timestamp = millis();

#ifdef LOG_SERIAL
  Serial.println(millis());
#endif

  delay(100);

  for (int i=0;i<sensor_SIZE;i++){

    A_Values[i] = 1023-analogRead(sensorPins[i][0]); //Attention à corriger ça
  }

  for (int i=0;i<sensor_SIZE;i++){

    pinMode(sensorPins[i][0], OUTPUT);
    digitalWrite(sensorPins[i][0], LOW);
    digitalWrite(sensorPins[i][1], HIGH);
  }

#ifdef LOG_SERIAL
  Serial.println(millis());
  Serial.println((unsigned long)(millis() - A_values_timestamp));
#endif

  unsigned long timeWithCurrent = (unsigned long)(millis() - A_values_timestamp);
  delay(timeWithCurrent);

  for (int i=0;i<sensor_SIZE;i++){

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

  delay((unsigned long)15*60*1000 - 2*timeWithCurrent); // 15 min de pause
}

void requestEvent()
{
  int bufferSize = sizeof(int)*(sensor_SIZE)+sizeof(long);
  byte buffer[bufferSize];

  for (int i=0;i<sensor_SIZE;i++){
    intToByte(A_Values[i], &buffer[sizeof(int)*i]);
  }
  longToByte((unsigned long)(millis() - A_values_timestamp),&buffer[sizeof(int)*(sensor_SIZE)]);

  Wire.write(buffer, bufferSize);
}


void receiveEvent(int len){

}

void intToByte(unsigned int value, byte *buffer){
  for (int i = 0;i<sizeof(int);i++){
    buffer[i] = value >> ((sizeof(int)-1-i)*8);
  }
}

void longToByte(unsigned long value, byte *buffer){
  for (int i = 0;i<sizeof(long);i++){
    buffer[i] = value >> ((sizeof(long)-1-i)*8);
  }
}








