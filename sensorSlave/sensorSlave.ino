#include <Wire.h>
#include <EEPROM.h>

/***************************************/
/*     Config                          */
/***************************************/

#define DEFAULT_I2C_ADRESS 3           
#define NUMBER_OF_SENSORS 4  // 4 on mini,uno - 6 on promini

//#define LOG_SERIAL
//#define FAKESENSORSWITHCOUNTERS

/***************************************/

int i2cAdress = DEFAULT_I2C_ADRESS;

const int allSsensorPins[][2] = {
  {
    A0,5                              }
  ,{
    A1,4                              }
  ,{
    A2,3                              }
  ,{
    A3,2                              }
  ,{
    A6,6                              }
  ,{
    A7,7                              }

};

const byte sensor_SIZE = NUMBER_OF_SENSORS;
int sensorPins[sensor_SIZE][2];
unsigned int A_Values[sensor_SIZE];
unsigned long A_values_timestamp = 0;

volatile unsigned long A_Values_Period = 1000;
volatile int last_command = 0;

#define COMMAND_READ_SENSORS 0
#define COMMAND_SET_ADRESS 1
#define COMMAND_SET_PERIOD 2
#define COMMAND_GET_PERIOD 3

void setup()
{  

  for (int i=0;i<sensor_SIZE;i++){
    sensorPins[i][0] = allSsensorPins[i][0];
    sensorPins[i][1] = allSsensorPins[i][1];
  }

#ifdef LOG_SERIAL
  Serial.begin(9600);
  Serial.println("booting");
#endif

  i2cAdress = readLongFromEEPROM(5);
  if (i2cAdress < 3 || i2cAdress > 77)
    i2cAdress = DEFAULT_I2C_ADRESS;
  Wire.begin(i2cAdress);

#ifdef LOG_SERIAL
Serial.print("i2c adress ");
  Serial.println(i2cAdress); 
#endif


  A_Values_Period = readLongFromEEPROM(1);
#ifdef LOG_SERIAL
  Serial.print("measure period "); 
  Serial.println(A_Values_Period); 
#endif

  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  Serial.print("setup finished"); 
}

void loop()
{
#ifdef FAKESENSORSWITHCOUNTERS

  A_Values[0] = millis();
  A_Values[1] = millis()/1000;      //seconds
  A_Values[2] = millis()/1000/60;   //minutes
  A_Values[3] = millis()/1000/60/60;//hours

  A_values_timestamp = millis();

#ifdef LOG_SERIAL
  for (int i=0;i<sensor_SIZE;i++){
    Serial.print(A_Values[i]);    
    Serial.print("|");
  }
  Serial.println("-------------------------------------");

#endif
  delay(A_Values_Period+1000);

#else
  for (int i=0;i<sensor_SIZE;i++){

    pinMode(sensorPins[i][0], INPUT_PULLUP);
    pinMode(sensorPins[i][1], OUTPUT);
    digitalWrite(sensorPins[i][1], LOW);
  }

  A_values_timestamp = millis();

  delay(100);

  for (int i=0;i<sensor_SIZE;i++){
    A_Values[i] = 1023-analogRead(sensorPins[i][0]); //Attention à corriger ça
  }

  for (int i=0;i<sensor_SIZE;i++){

    pinMode(sensorPins[i][0], OUTPUT);
    digitalWrite(sensorPins[i][0], LOW);
    digitalWrite(sensorPins[i][1], HIGH);
  }

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

  delay(A_Values_Period - 2*timeWithCurrent);

#endif
}

void requestEvent()
{ 
  switch (last_command){

  case COMMAND_READ_SENSORS:
    writeSensorsAsBytes();
    break;

  case COMMAND_GET_PERIOD:
    writePeriodAsBytes();
    break;
  }

  last_command = -1;
}


void receiveEvent(int len){
  if(Wire.available()){
    last_command = Wire.read(); 
  }

  switch (last_command){
  case COMMAND_SET_PERIOD:
    A_Values_Period = readLong();
    writeLongInEEPROM(1, A_Values_Period);
    asm volatile ("  jmp 0");
    break;

  case COMMAND_SET_ADRESS:
    i2cAdress = readLong();
    writeLongInEEPROM(5, i2cAdress);
    asm volatile ("  jmp 0");
    break;
  }
}

/* Replies to request */

// COMMAND_READ_SENSORS
void writeSensorsAsBytes(){
  int bufferSize = sizeof(long)+sizeof(byte)+sizeof(int)*(sensor_SIZE)+sizeof(byte);
  byte buffer[bufferSize];

  longToByte((unsigned long)(millis() - A_values_timestamp),buffer);
  buffer[sizeof(long)] = (byte)sensor_SIZE;
  
  for (int i=0;i<sensor_SIZE;i++){
    intToByte(A_Values[i], &buffer[sizeof(long)+sizeof(byte)+sizeof(int)*i]);
  }
  
  buffer[bufferSize-1] = 0x00; //semaphore

  Wire.write(buffer, bufferSize); 
}

// COMMAND_GET_PERIOD
void writePeriodAsBytes(){
  byte buffer[sizeof(long)];
  longToByte(A_Values_Period, buffer);

  Wire.write(buffer, sizeof(long));    
}

/* receive datas */

//Read datas from i2c
long readLong(){
  byte buffer[sizeof(long)];
  int i = 0;

  while(Wire.available()){
    buffer[i++] = Wire.read();
  }

  return ByteToLong(buffer);
}

//Read datas from EEPROM
long readLongFromEEPROM(int firstByte){
  byte buffer[sizeof(long)];
  for (int i = 0;i<sizeof(long);i++){
    buffer[i] = EEPROM.read(firstByte+i);
#ifdef LOG_SERIAL
    Serial.println(buffer[i]);
#endif
  }
  return ByteToLong(buffer);
}

//Write data to EEPROM
void writeLongInEEPROM(int firstByte, long value){
  byte buffer[sizeof(long)];
  longToByte(value,buffer);
  for (int i = 0;i<sizeof(long);i++){
    EEPROM.write(firstByte+i,buffer[i]);
#ifdef LOG_SERIAL
    Serial.println(buffer[i]);
#endif
  }
}

//bytes utilities
void intToByte(unsigned int value, byte *buffer){
  for (int i = 0;i<sizeof(int);i++){
    buffer[i] = value >> ((sizeof(int)-1-i)*8);
  }
}

int byteToInt(byte *buffer){
  unsigned int result = 0;
  for (int i = 0;i<sizeof(int);i++){
    result += ((unsigned int)buffer[i]) << ((sizeof(int)-1-i)*8);
  }
  return result;
}

void longToByte(unsigned long value, byte *buffer){
  for (int i = 0;i<sizeof(long);i++){
    buffer[i] = value >> ((sizeof(long)-1-i)*8);
  }
}

long ByteToLong(byte *buffer){
  unsigned long result = 0;
  for (int i = 0;i<sizeof(long);i++){
    result += ((unsigned long)buffer[i]) << ((sizeof(long)-1-i)*8);
  }
  return result;
}
















