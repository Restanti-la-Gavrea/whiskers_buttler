#include<SPI.h>                         //Library for SPI  
byte sendData[] = {0xAB, 0xCD, 0xEF};
byte recData[3];


void setup (void){

  Serial.begin(115200);                   //Starts Serial Communication at Baud Rate 115200 
  SPI.begin();                            //Begins the SPI commnuication
  digitalWrite(SS, LOW);
  
}
void loop()
{
  for (int i = 0; i < 3; i++)
  {
    recData[i] = SPI.transfer(sendData[i]);
    Serial.print("recData["); Serial.print(i); Serial.print("] = ");
    Serial.print(recData[i], HEX);
    Serial.print(" from Slave's sData[");
    Serial.print(i-1);
    Serial.println("] ");
  }
  long y = (long)recData[1] << 16 | (long)recData[2] << 8 | (long)recData[0];
  Serial.println(y, HEX);
  Serial.print("Data Received from Slave: ");
  Serial.println("=========================");
  delay(3000);
}



#include<SPI.h>