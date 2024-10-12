// Joystick
#define x 13
#define y 12
#define bot 14

// OUTPUT Signals q
#define izq 27
#define der 26
#define up 25
#define down 33
#define boom 32

// Funciones
void controlMotor();
// void pmwBegin();


void setup() {
  Serial.begin(9600);

  // pwmBegin();
  analogWrite(bot, 255);  
}

void loop() {
  controlMotor();
  delay(10);
  Serial.print("x: ");
  Serial.print(analogRead(x));
  Serial.print("\t y: ");
  Serial.print(analogRead(y));
  Serial.print("\t bot: ");
  Serial.println(analogRead(bot));

  // analogWrite(izq, 0);
  // analogWrite(der, 0);
  // analogWrite(up, 0);
  // analogWrite(down, 0);
  // analogWrite(boom, 0);
}


void controlMotor() {
  // int x_value, y_value, bot_value;

  int x_value = map(analogRead(x), 0, 4096, 0, 255);
  int y_value = map(analogRead(y), 0, 4096, 0, 255);
  int bot_value = map(analogRead(bot), 0, 4096, 0, 255);

  if ((x_value < (255 / 2) - 30) or (x_value > (255 / 2) + 30))
  {
    analogWrite(izq, 0);
    analogWrite(der, 0);
  }
  if ((y_value < (255 / 2) - 30) or (y_value > (255 / 2) + 30))
  {
    analogWrite(up, 0);
    analogWrite(down, 0);
  }

  if (x_value < (255 / 2) - 50) {
    analogWrite(izq, - x_value + 255);
  }

  if (x_value > (255 / 2) + 50)
    analogWrite(der, x_value);

  if (y_value < (255 / 2) - 50)
    analogWrite(up, - y_value + 255);

  if (y_value > (255 / 2) + 50)
    analogWrite(down, y_value);

  if (bot_value = 0)
    analogWrite(boom, 255);
}

// void pwmBegin() {
//   // ledcSetup(canal, frecuencia, resolucion);

//   // Pins de salida:
//   // ledcAttachPin(izq, OUTPUT);
//   // ledcAttachPin(der, OUTPUT);
//   // ledcAttachPin(up, OUTPUT);
//   // ledcAttachPin(down, OUTPUT);
// }