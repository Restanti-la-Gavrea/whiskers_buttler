#include "LineDetector.h"

#define CLP ((int) 0x000020)
void LineDetector::config(){
  pinMode(S1_PIN,INPUT);
  pinMode(S2_PIN,INPUT);
  pinMode(S3_PIN,INPUT);
  pinMode(S4_PIN,INPUT);
  pinMode(S5_PIN,INPUT);
  pinMode(CLP_PIN,INPUT);
}
 

uint8_t LineDetector::read(){
  uint8_t s1 = !digitalRead(S1_PIN);// 1 if hit the line, 0 if not
  uint8_t s2 = !digitalRead(S2_PIN);
  uint8_t s3 = !digitalRead(S3_PIN);
  uint8_t s4 = !digitalRead(S4_PIN);
  uint8_t s5 = !digitalRead(S5_PIN);
  line = ( (s1 <<4) | (s2<<3) | (s3 << 2) | (s4 <<1 ) | s5) & 0x00003F;

    if(getLine()) {
        uint8_t nr = s1+ s2 + s3 + s4 + s5;
        relativePosition = ((s1-s5)*4 + (s2-s4)*2)/nr;
    }else{
        relativePosition = 100;
    }
  return line;
}
void LineDetector::printLine(){
    Serial.print("line = ");
    for (int8_t aBit = 4; aBit >= 0; aBit--)
      Serial.print((line & 1 << aBit)!=0);
    Serial.print(", relativePosition = ");
    Serial.print(relativePosition);
    Serial.print(", isInLine = ");
    uint8_t is = getLine();
    Serial.println(is);
}

uint8_t LineDetector::getLine(){
  return line ;
}
