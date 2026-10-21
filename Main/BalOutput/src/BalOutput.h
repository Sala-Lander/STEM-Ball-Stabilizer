#ifndef BALOUTPUT_H
#define BALOUTPUT_H

#include <Arduino.h>
#include <Servo.h>

class BalOutput {
 public:
  void begin(int pinX, int pinY);

  void zetUitslag(float uitslagX, float uitslagY);

  void vlak();

  void zetMidden(int middenX, int middenY);
  void zetMaxUitslag(float graden);
  void keerOm(bool omkeerX, bool omkeerY);

 private:
  Servo _servoX; 
  Servo _servoY;

  int _middenX = 90;
  int _middenY = 90;
  float _maxUitslag = 40;
  bool _omkeerX = true;
  bool _omkeerY = false;
};

#endif
