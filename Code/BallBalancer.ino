#include <BalInput.h>
#include <BalOutput.h>

const int PIN_SERVO_X = 11;
const int PIN_SERVO_Y = 10;

//KALIBRATIE  --> AANPASSEN! Meet ze met de sketch "Kalibratie".

const int RAW_MIN_X = 200;       // ruwe waarde helemaal links
const int RAW_MAX_X = 3900;      // ruwe waarde helemaal rechts
const int RAW_MIN_Y = 200;       // ruwe waarde onderaan
const int RAW_MAX_Y = 3900;      // ruwe waarde bovenaan
const float HALF_BREEDTE = 85;   // helft van de breedte van je paneel (mm)
const float HALF_HOOGTE = 64;    // helft van de hoogte van je paneel (mm)
const int MIN_DRUK = 100;        // minimale druk

// PID-INSTELLINGEN (startwaarden van de maker)
// Kp = reageer op HOE VER de bal van het doel is
// Ki = reageer op hoe LANG de bal al ernaast ligt
// Kd = reageer op hoe SNEL de bal beweegt (remt af)

const float Kp = 0.2;
const float Ki = 0.05;
const float Kd = 0.1;

const float MAX_UITSLAG = 40;
const float MAX_SOM = 500;

//   0 = bal in het midden houden
//   1 = bal in een cirkel laten rollen

const int PATROON = 0;
const float CIRKEL_STRAAL = 30;
const float CIRKEL_TIJD = 8;

const bool DEBUG = false;

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

// regel(): de PID-regelaar voor EEN as.
//   as      = 0 voor X, 1 voor Y
//   dt      = hoeveel seconden de vorige ronde duurde

float regel(int as, float doel, float positie, float dt) {
  float fout = doel - positie;

  float deelP = Kp * fout;

  integraal[as] = integraal[as] + fout * dt;
  integraal[as] = constrain(integraal[as], -MAX_SOM, MAX_SOM);
  float deelI = Ki * integraal[as];

  float snelheid = (positie - vorigePositie[as]) / dt;
  float deelD = -Kd * snelheid;
  vorigePositie[as] = positie;

  return deelP + deelI + deelD;
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

  // Kantelt het platform de verkeerde kant op? Probeer:
  //   uitvoer.keerOm(false, false);   (of true, true / enz.)
  // Staat het platform niet waterpas bij 90 graden? Probeer:
  //   uitvoer.zetMidden(92, 88);      (jouw eigen waarden)

  vorigeTijd = millis();
}

void loop() {
  unsigned long nu = millis();
  float dt = (nu - vorigeTijd) / 1000.0;
  vorigeTijd = nu;

  BalPositie bal = invoer.lees();

  if (bal.aanwezig) {
    if (!balAanwezig) {
      vorigePositie[0] = bal.x;
      vorigePositie[1] = bal.y;
      integraal[0] = 0;
      integraal[1] = 0;
      balAanwezig = true;
    }

    kiesDoel();

    float uitslagX = regel(0, doelX, bal.x, dt);
    float uitslagY = regel(1, doelY, bal.y, dt);

    uitvoer.zetUitslag(uitslagX, uitslagY);

    if (DEBUG) {
      Serial.print("bal x=");  Serial.print(bal.x);
      Serial.print(" y=");     Serial.print(bal.y);
      Serial.print(" | uitslag X="); Serial.print(uitslagX);
      Serial.print(" Y=");     Serial.println(uitslagY);
    }
  }
  else {
    uitvoer.vlak();
    balAanwezig = false;
  }
}
