#ifndef BALINPUT_H
#define BALINPUT_H

#include <Arduino.h>
#include <TouchScreen.h>

struct BalPositie {
  bool aanwezig;
  float x;
  float y;
};

class BalInput {
public:
  BalInput();

  bool begin();

  void kalibreer(
    int rawMinX,
    int rawMaxX,
    int rawMinY,
    int rawMaxY,
    float halfBreedte,
    float halfHoogte
  );

  void zetMinDruk(int minDruk);

  BalPositie lees();

private:
  static const int XP = A0;
  static const int XM = A1;
  static const int YP = A2;
  static const int YM = A3;

  TouchScreen _touch;

  int _rawMinX = 0;
  int _rawMaxX = 1023;
  int _rawMinY = 0;
  int _rawMaxY = 1023;

  float _halfBreedte = 85.0f;
  float _halfHoogte = 64.0f;

  int _minDruk = 10;
};

#endif
