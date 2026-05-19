// Definim un array amb els pins analògics on estan connectats els 5 faders
const int FADER_PINS[5]  = { A0, A1, A2, A3, A5 };

// Definim un array amb els pins digitals on estan connectats els 4 botons
const int BUTTON_PINS[4] = { 2, 3, 4, 5 };

// Array per guardar els valors actuals dels faders
int  faderValues[5] = {0};

// Array que guarda l'estat actual dels botons (true = premut)
bool buttonState[4] = {false};

// Array per guardar l'estat anterior dels botons
// (en aquest codi no s'utilitza encara, però pot servir per detectar canvis)
bool buttonPrev[4]  = {false};

// Variable per controlar cada quant temps s'envien dades pel Serial
unsigned long lastSend = 0;

// Interval d'enviament en mil·lisegons (50 ms)
const unsigned long SEND_INTERVAL = 50;


// ---------------------- SETUP ----------------------
void setup() {

  // Inicialitza la comunicació Serial a 115200 bauds
  Serial.begin(115200);

  // Configurem els 4 botons com INPUT_PULLUP
  // Això activa la resistència interna pull-up:
  // - LOW = botó premut
  // - HIGH = botó no premut
  for (int i = 0; i < 4; i++)
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);

  // Llegim l'estat inicial de tots els faders i botons
  readAll();

  // Enviem l'estat inicial pel Serial
  sendState();
}


// ---------------------- LOOP ----------------------
void loop() {

  // Llegim constantment tots els faders i botons
  readAll();

  // Comprovem si han passat 50 ms des de l'últim enviament
  if (millis() - lastSend >= SEND_INTERVAL) {

    // Actualitzem el temps de l'últim enviament
    lastSend = millis();

    // Enviem les dades pel Serial
    sendState();
  }
}


// ---------------------- LLEGIR DADES ----------------------
void readAll() {

  // -------- LLEGIR FADERS --------
  for (int i = 0; i < 5; i++) {

    // Variable temporal per sumar lectures
    int raw = 0;

    // Fem 4 lectures del mateix fader
    // per reduir soroll i obtenir una lectura més estable
    for (int s = 0; s < 4; s++)
      raw += analogRead(FADER_PINS[i]);

    // Fem la mitjana de les 4 lectures
    faderValues[i] = raw / 4;
  }


  // -------- LLEGIR BOTONS --------
  for (int i = 0; i < 4; i++) {

    // digitalRead retorna:
    // LOW  -> botó premut
    // HIGH -> botó no premut
    // Guardem true si està premut
    buttonState[i] = (digitalRead(BUTTON_PINS[i]) == LOW);
  }
}


// ---------------------- ENVIAR DADES ----------------------
void sendState() {

  // Comencem a enviar un objecte JSON:
  // {"f":[...],"b":[...]}

  // Inici de la part dels faders
  Serial.print(F("{\"f\":["));

  // Enviem els 5 valors dels faders
  for (int i = 0; i < 5; i++) {

    // Enviem el valor del fader
    Serial.print(faderValues[i]);

    // Afegim una coma entre valors
    if (i < 4)
      Serial.print(',');
  }

  // Inici de la part dels botons
  Serial.print(F("],\"b\":[")); 

  // Enviem l'estat dels 4 botons
  for (int i = 0; i < 4; i++) {

    // Si el botó està premut enviem 1
    // Si no, enviem 0
    Serial.print(buttonState[i] ? 1 : 0);

    // Afegim comes entre valors
    if (i < 3)
      Serial.print(',');
  }

  // Tanquem el JSON i fem salt de línia
  Serial.println(F("]}"));
}