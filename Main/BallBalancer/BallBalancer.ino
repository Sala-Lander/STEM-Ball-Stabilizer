#include <BalInput.h>
#include <BalOutput.h>

const int PIN_SERVO_X = 11;
const int PIN_SERVO_Y = 10;

const int RAW_MIN_X = 200;       // ruwe waarde helemaal links
const int RAW_MAX_X = 3900;      // ruwe waarde helemaal rechts
const int RAW_MIN_Y = 200;       // ruwe waarde onderaan
const int RAW_MAX_Y = 3900;      // ruwe waarde bovenaan
const float HALF_BREEDTE = 85;   // helft van de breedte van je paneel (mm)
const float HALF_HOOGTE = 64;    // helft van de hoogte van je paneel (mm)
const int MIN_DRUK = 100;        // minimale druk

const float Kp = 0.2;
const float Ki = 0.05;
const float Kd = 0.1;

const float MAX_UITSLAG = 40;
const float MAX_SOM = 500;

const int PATROON = 0; // 0 = midden, 1 = cirkel

const float CIRKEL_STRAAL = 30;
const float CIRKEL_TIJD = 8;

const bool DEBUG = false;

const int AS_X = 0;
const int AS_Y = 1;

BalInput invoer;
BalOutput uitvoer;

float doelX = 0;
float doelY = 0;

float vorigePositie[2];

float integraal[2];

bool balAanwezig = false;

unsigned long vorigeTijd = 0;

void kiesDoel() {
  if (PATROON == 0) {
    doelX = 0;
    doelY = 0;
  }
  else if (PATROON == 1) {
    float seconden = millis() / 1000.0;
    float hoek = 2 * PI * seconden / CIRKEL_TIJD;

    doelX = CIRKEL_STRAAL * cos(hoek);
    doelY = CIRKEL_STRAAL * sin(hoek);
  }
}

float berekenTijdStap() {
  unsigned long huidigeTijd = millis();

  float tijdStap = (huidigeTijd - vorigeTijd) / 1000.0;

  vorigeTijd = huidigeTijd;

  return tijdStap;
}

void startRegelaarOpBal(BalPositie bal) {
  vorigePositie[AS_X] = bal.x;
  vorigePositie[AS_Y] = bal.y;

  integraal[AS_X] = 0;
  integraal[AS_Y] = 0;

  balAanwezig = true;
}

float berekenUitslagVoorAs(int asNummer, float doel, float positie, float tijdStap) {
  float fout = doel - positie;

  float deelP = Kp * fout;

  integraal[asNummer] = integraal[asNummer] + fout * tijdStap;
  integraal[asNummer] = constrain(integraal[asNummer], -MAX_SOM, MAX_SOM);
  float deelI = Ki * integraal[asNummer];

  float snelheid = (positie - vorigePositie[asNummer]) / tijdStap;
  float deelD = -Kd * snelheid;

  vorigePositie[asNummer] = positie;

  return deelP + deelI + deelD;
}

void toonDebugInfo(BalPositie bal, float uitslagX, float uitslagY) {
  Serial.print("bal x=");  Serial.print(bal.x);
  Serial.print(" y=");     Serial.print(bal.y);
  Serial.print(" | uitslag X=");  Serial.print(uitslagX);
  Serial.print(" Y=");     Serial.println(uitslagY);
}

void setup() {
  Serial.begin(115200);

  if (!invoer.begin()) {
    Serial.println("Touchscreen-module niet gevonden!");
    Serial.println("Controleer: SDA naar A4, SCL naar A5, voeding en GND.");
    while (true) {
    }
  }

  invoer.kalibreer(RAW_MIN_X, RAW_MAX_X, RAW_MIN_Y, RAW_MAX_Y,
                   HALF_BREEDTE, HALF_HOOGTE);
  invoer.zetMinDruk(MIN_DRUK);

  uitvoer.begin(PIN_SERVO_X, PIN_SERVO_Y);
  uitvoer.zetMaxUitslag(MAX_UITSLAG);

  vorigeTijd = millis();
}

void loop() {
  float tijdStap = berekenTijdStap();

  BalPositie bal = invoer.lees();

  if (bal.aanwezig) {
    if (balAanwezig == false) {
      startRegelaarOpBal(bal);
    }

    kiesDoel();

    float uitslagX = berekenUitslagVoorAs(AS_X, doelX, bal.x, tijdStap);
    float uitslagY = berekenUitslagVoorAs(AS_Y, doelY, bal.y, tijdStap);

    uitvoer.zetUitslag(uitslagX, uitslagY);

    if (DEBUG) {
      toonDebugInfo(bal, uitslagX, uitslagY);
    }
  }
  else {
    uitvoer.vlak();
    balAanwezig = false;
  }
}
