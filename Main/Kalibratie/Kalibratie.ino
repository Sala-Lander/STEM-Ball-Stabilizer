#include <TouchScreen.h>

// Bedrading:
// X+ = A0
// X- = A1
// Y+ = A2
// Y- = A3

#define XP A0
#define XM A1
#define YP A2
#define YM A3

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

const int MIN_DRUK = 10;

int minX = 1023;
int maxX = 0;
int minY = 1023;
int maxY = 0;

void setup() {
  Serial.begin(115200);

  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);

  Serial.println("Touchscreen-kalibratie gestart.");
  Serial.println("Raak de uiterste randen van het scherm aan.");
  Serial.println("Beweeg je vinger langs alle vier de kanten.");
  Serial.println();
}

void loop() {
  TSPoint p = ts.getPoint();

  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);

  digitalWrite(YP, LOW);
  digitalWrite(XM, LOW);

  if (p.z >= MIN_DRUK) {
    if (p.x < minX) minX = p.x;
    if (p.x > maxX) maxX = p.x;

    if (p.y < minY) minY = p.y;
    if (p.y > maxY) maxY = p.y;

    Serial.print("X: ");
    Serial.print(p.x);

    Serial.print(" | Y: ");
    Serial.print(p.y);

    Serial.print(" | Druk: ");
    Serial.println(p.z);

    Serial.print("X_MIN=");
    Serial.print(minX);

    Serial.print(" | X_MAX=");
    Serial.print(maxX);

    Serial.print(" | Y_MIN=");
    Serial.print(minY);

    Serial.print(" | Y_MAX=");
    Serial.println(maxY);

    Serial.println();
  }

  delay(20);
}
