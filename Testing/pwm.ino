void setup() {
  Serial.begin(9600);
}

void loop() {
  int contador = 0;
  for (contador = 0; contador < 255; contador++) {
    analogWrite(25, contador);
    delay(8);
    Serial.print("Contador: ");
    Serial.println(contador);
  }
  for (contador = 255; contador > 0; contador--) {
    analogWrite(25, contador);
    delay(8);
    Serial.print("Contador: ");
    Serial.println(contador);
  }
}