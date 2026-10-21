#include "BalOutput.h"

void BalOutput::begin(int pinX, int pinY) {
  _servoX.attach(pinX);  
  _servoY.attach(pinY);
  vlak();
}

void BalOutput::zetUitslag(float uitslagX, float uitslagY) {
  uitslagX = constrain(uitslagX, -_maxUitslag, _maxUitslag);
  uitslagY = constrain(uitslagY, -_maxUitslag, _maxUitslag);

  if (_omkeerX) {
    uitslagX = -uitslagX;
  }
  if (_omkeerY) {
    uitslagY = -uitslagY;
  }

  int hoekX = constrain((int)round(_middenX + uitslagX), 0, 180);
  int hoekY = constrain((int)round(_middenY + uitslagY), 0, 180);

  _servoX.write(hoekX);
  _servoY.write(hoekY);
}

void BalOutput::vlak() {
  zetUitslag(0, 0);
}

void BalOutput::zetMidden(int middenX, int middenY) {
  _middenX = middenX;
  _middenY = middenY;
}

void BalOutput::zetMaxUitslag(float graden) {
  _maxUitslag = graden;
}

void BalOutput::keerOm(bool omkeerX, bool omkeerY) {
  _omkeerX = omkeerX;
  _omkeerY = omkeerY;
}
