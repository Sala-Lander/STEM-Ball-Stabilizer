#include <stdint.h>
#include "TouchScreen.h"

#define YP A2  
#define XM A3  
#define YM A0  
#define XP A1  

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

const int RAUW_X_MIN = 120;  // Max. X -> Links
const int RAUW_X_MAX = 880;  // Max. X -> Rechts
const int RAUW_Y_MIN = 150;  // Max. Y -> Onder
const int RAUW_Y_MAX = 850;  // Max. Y -> Boven

const float SCHERM_BREEDTE_MM = 150.0; 
const float SCHERM_HOOGTE_MM  = 100.0; 

float mmX = 0.0;
float mmY = 0.0;

int laatsteRauwX = 512;
int laatsteRauwY = 512;

void setup() {
  Serial.begin(115200); 
}

void loop() {
  TSPoint p = ts.getPoint();
  
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);

  if (p.z > 10) { 
    laatsteRauwX = p.x;
    laatsteRauwY = p.y;
  }

  berekenMillimeters(laatsteRauwX, laatsteRauwY);

  Serial.print("X_mm:");
  Serial.print(mmX);
  Serial.print(",");
  Serial.print("Y_mm:");
  Serial.println(mmY);

  delay(20); 
}

// --- DE EXTRA FUNCTIE VOOR MILLIMETERS ---
void berekenMillimeters(int rauwX, int rauwY) {
  // We gebruiken de Arduino 'map' functionaliteit, maar dan met float (kommagetallen) 
  // voor maximale precisie. We berekenen eerst de positie van 0 tot de maximale schermgrootte.
  float gecorrigeerdX = (float)(rauwX - RAUW_X_MIN) * SCHERM_BREEDTE_MM / (float)(RAUW_X_MAX - RAUW_X_MIN);
  float gecorrigeerdY = (float)(rauwY - RAUW_Y_MIN) * SCHERM_HOOGTE_MM / (float)(RAUW_Y_MAX - RAUW_Y_MIN);

  // Voor een balancing robot is het 't handigst als het exacte midden van het scherm (0, 0) is.
  // Als de bal links van het midden ligt wordt het X negatief, rechts wordt X positief.
  mmX = gecorrigeerdX - (SCHERM_BREEDTE_MM / 2.0);
  mmY = gecorrigeerdY - (SCHERM_HOOGTE_MM / 2.0);
}