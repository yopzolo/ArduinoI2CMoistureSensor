#include <Wire.h>

void setup()
{
  Wire.begin();        // join i2c bus (address optional for master)
  Serial.begin(9600);  // start serial for output
}

  void loop()
  {
    Wire.requestFrom(2, 12);    // request 6 bytes from slave device #2
  
    while(Wire.available())    // slave may send less than requested
    {
      byte high = Wire.read();
      byte low = Wire.read();      
      unsigned int value = (high << 8) + low;
      Serial.print(value);    
      Serial.print("|");
    }
      Serial.println();
    delay(75);
  }
