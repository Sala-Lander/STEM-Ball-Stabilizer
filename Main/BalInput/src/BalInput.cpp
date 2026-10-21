#include "BalInput.h"

static const int AANTAL_METINGEN = 5;
static const int MIN_GELDIG = 3;

static int mediaan(int lijst[], int aantal) {
  for (int i = 0; i < aantal - 1; i++) {
    for (int j = 0; j < aantal - 1 - i; j++) {
      if (lijst[j] > lijst[j + 1]) {
        int tijdelijk = lijst[j];
        lijst[j] = lijst[j + 1];
        lijst[j + 1] = tijdelijk;
      }
    }
  }

  return lijst[aantal / 2];
}

static float naarMm(
  int ruw,
  int ruwMin,
  int ruwMax,
  float halfGrootte
) {
  if (ruwMax == ruwMin) {
    return 0.0f;
  }

  float deel =
    (float)(ruw - ruwMin) / (float)(ruwMax - ruwMin);

  return (deel * 2.0f - 1.0f) * halfGrootte;
}

BalInput::BalInput()
  : _touch(XP, YP, XM, YM, 300) {
}

bool BalInput::begin() {
  pinMode(YP, OUTPUT);
  pinMode(XM, OUTPUT);

  digitalWrite(YP, LOW);
  digitalWrite(XM, LOW);

  return true;
}

void BalInput::kalibreer(
  int rawMinX,
  int rawMaxX,
  int rawMinY,
  int rawMaxY,
  float halfBreedte,
  float halfHoogte
) {
  _rawMinX = rawMinX;
  _rawMaxX = rawMaxX;
  _rawMinY = rawMinY;
  _rawMaxY = rawMaxY;

  _halfBreedte = halfBreedte;
  _halfHoogte = halfHoogte;
}

void BalInput::zetMinDruk(int minDruk) {
  _minDruk = minDruk;
}

BalPositie BalInput::lees() {
  BalPositie antwoord;

  antwoord.aanwezig = false;
  antwoord.x = 0.0f;
  antwoord.y = 0.0f;

  int lijstX[AANTAL_METINGEN];
  int lijstY[AANTAL_METINGEN];

  int geldig = 0;

  for (int n = 0; n < AANTAL_METINGEN; n++) {
    TSPoint p = _touch.getPoint();

    pinMode(YP, OUTPUT);
    pinMode(XM, OUTPUT);

    digitalWrite(YP, LOW);
    digitalWrite(XM, LOW);

    if (p.z >= _minDruk && geldig < AANTAL_METINGEN) {
      lijstX[geldig] = p.x;
      lijstY[geldig] = p.y;
      geldig++;
    }

    delay(2);
  }

  if (geldig < MIN_GELDIG) {
    return antwoord;
  }

  int ruwX = mediaan(lijstX, geldig);
  int ruwY = mediaan(lijstY, geldig);

  antwoord.x = naarMm(
    ruwX,
    _rawMinX,
    _rawMaxX,
    _halfBreedte
  );

  antwoord.y = naarMm(
    ruwY,
    _rawMinY,
    _rawMaxY,
    _halfHoogte
  );

  antwoord.aanwezig = true;

  return antwoord;
}
