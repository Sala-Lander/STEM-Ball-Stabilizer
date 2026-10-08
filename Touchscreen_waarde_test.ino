#include <stdint.h>
#include "TouchScreen.h"


#define YP A2  
#define XM A3  
#define YM A0  
#define XP A1  

// Weerstandswaarde (meestal ~300 ohm)
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int laatsteX = 512;
int laatsteY = 512;

void setup() {
  Serial.begin(115200); 
}

void loop() {
  TSPoint p = ts.getPoint();
  
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);

 
  // 10 aanpassen aan gewicht bal
  if (p.z > 10) { 
    
    laatsteX = p.x;
    laatsteY = p.y;
    
    Serial.print("X_Positie:");
    Serial.print(laatsteX);
    Serial.print(",");
    Serial.print("Y_Positie:");
    Serial.println(laatsteY);

  } else {
    Serial.print("X_Positie:");
    Serial.print(laatsteX);
    Serial.print(",");
    Serial.print("Y_Positie:");
    Serial.println(laatsteY);
  }
  delay(20); 
}