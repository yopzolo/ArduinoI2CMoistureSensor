#include <Wire.h>

void setup()
{
  
  //TODO make adress configurable with digital pins
  Wire.begin(2);
  Wire.onRequest(requestEvent);
}

unsigned int A_Values[6] = {
  0,0,0,0,0,0};

void loop()
{
  delay(50);

  A_Values[0]++;
  for (int i=0;i<5;i++){
    if (A_Values[i]%3 == 0)
      A_Values[i+1]++;
    else
      break;
  }
}

void requestEvent()
{
  byte buffer[12];

  for (int i=0;i<6;i++){
    buffer[2*i] = A_Values[i] >> 8;
    buffer[2*i+1] = A_Values[i] & 0xff;
  }

  Wire.write(buffer, 12);
}











