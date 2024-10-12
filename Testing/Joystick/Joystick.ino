// Joystick
#define x 13
#define y 12
#define bot 14

void setup() {
  Serial.begin(9600);

  analogWrite(bot, 255);
}

void loop() {
  Serial.print("x: ");
  Serial.print(analogRead(x));
  Serial.print("\t y: ");
  Serial.print(analogRead(y));
  Serial.print("\t bot: ");
  Serial.println(analogRead(bot));
  delay(50);
}