const int FADER_PINS[5]  = { A0, A1, A2, A3, A5 };
const int BUTTON_PINS[4] = { 2, 3, 4, 5 };

int  faderValues[5] = {0};
bool buttonState[4] = {false};
bool buttonPrev[4]  = {false};

unsigned long lastSend = 0;
const unsigned long SEND_INTERVAL = 50;

void setup() {
  Serial.begin(115200);
  for (int i = 0; i < 4; i++)
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  readAll();
  sendState();
}

void loop() {
  readAll();
  if (millis() - lastSend >= SEND_INTERVAL) {
    lastSend = millis();
    sendState();
  }
}

void readAll() {
  for (int i = 0; i < 5; i++) {
    int raw = 0;
    for (int s = 0; s < 4; s++) raw += analogRead(FADER_PINS[i]);
    faderValues[i] = raw / 4;
  }
  for (int i = 0; i < 4; i++) {
    buttonState[i] = (digitalRead(BUTTON_PINS[i]) == LOW);
  }
}

void sendState() {
  Serial.print(F("{\"f\":["));
  for (int i = 0; i < 5; i++) {
    Serial.print(faderValues[i]);
    if (i < 4) Serial.print(',');
  }
  Serial.print(F("],\"b\":["));
  for (int i = 0; i < 4; i++) {
    Serial.print(buttonState[i] ? 1 : 0);
    if (i < 3) Serial.print(',');
  }
  Serial.println(F("]}"));
}
