const int PIN_X_PLUS = A0;
const int PIN_X_MIN  = A1;
const int PIN_Y_PLUS = A2;
const int PIN_Y_MIN  = A3;

int minX = 1023;
int maxX = 0;
int minY = 1023;
int maxY = 0;

void maakPinnenVrij() {
  pinMode(PIN_X_PLUS, INPUT);
  pinMode(PIN_X_MIN, INPUT);
  pinMode(PIN_Y_PLUS, INPUT);
  pinMode(PIN_Y_MIN, INPUT);
}

bool isAangeraakt() {
  pinMode(PIN_X_PLUS, OUTPUT);
  digitalWrite(PIN_X_PLUS, LOW);

  pinMode(PIN_X_MIN, INPUT);
  pinMode(PIN_Y_PLUS, INPUT_PULLUP);
  pinMode(PIN_Y_MIN, INPUT);

  delayMicroseconds(20);
  bool aangeraakt = (digitalRead(PIN_Y_PLUS) == LOW);

  maakPinnenVrij();
  return aangeraakt;
}

int leesRuweX() {
  pinMode(PIN_X_PLUS, OUTPUT);
  digitalWrite(PIN_X_PLUS, HIGH);
  pinMode(PIN_X_MIN, OUTPUT);
  digitalWrite(PIN_X_MIN, LOW);

  pinMode(PIN_Y_PLUS, INPUT);
  pinMode(PIN_Y_MIN, INPUT);

  delayMicroseconds(20);
  int waarde = analogRead(PIN_Y_PLUS);

  maakPinnenVrij();
  return waarde;
}

int leesRuweY() {
  pinMode(PIN_Y_PLUS, OUTPUT);
  digitalWrite(PIN_Y_PLUS, HIGH);
  pinMode(PIN_Y_MIN, OUTPUT);
  digitalWrite(PIN_Y_MIN, LOW);

  pinMode(PIN_X_PLUS, INPUT);
  pinMode(PIN_X_MIN, INPUT);

  delayMicroseconds(20);
  int waarde = analogRead(PIN_X_PLUS);

  maakPinnenVrij();
  return waarde;
}

void setup() {
  Serial.begin(115200);
  maakPinnenVrij();

  Serial.println("Kalibratie van het 4-draads touchscreen");
  Serial.println("Aansluiting: X+ A0, X- A1, Y+ A2, Y- A3");
  Serial.println("Druk langs alle randen en hoeken van het scherm.");
  Serial.println("De waarden lopen van 0 tot 1023.");
  Serial.println();
}

void loop() {
  if (isAangeraakt()) {
    int x = leesRuweX();
    int y = leesRuweY();

    if (x < minX) minX = x;
    if (x > maxX) maxX = x;
    if (y < minY) minY = y;
    if (y > maxY) maxY = y;

    Serial.print("x=");
    Serial.print(x);
    Serial.print(" y=");
    Serial.print(y);

    Serial.print(" | minX=");
    Serial.print(minX);
    Serial.print(" maxX=");
    Serial.print(maxX);
    Serial.print(" minY=");
    Serial.print(minY);
    Serial.print(" maxY=");
    Serial.println(maxY);
  } else {
    Serial.println("geen aanraking");
  }

  delay(100);
}
