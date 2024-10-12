#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Joystick
#define m1PWM 13
#define m2PWM 12
#define rele1Motor1 14
#define rele2Motor1 27
#define rele3Motor2 26
#define rele4Motor2 25

uint8_t address[6] = { "mdca0" };

RF24 radio(4, 5);  //CSN and CE pins

unsigned long lastRecvTime = 0;

bool stateRele1y2 = false, stateRele3y4 = false, newStateRele1y2, newStateRele3y4;

// The sizeof this struct should not exceed 32 bytes
struct Received_data {
  byte ch1;  // Motor 1 - PWM - 0-255
  byte ch2;  // Motor 2 - PWM - 0-255
  byte ch3;  // Rele 1 y 2 - Motor1
  byte ch4;  // Rele 3 y 4 - Motor2
  byte ch5;  //
  byte ch6;  //
  byte ch7;  //
};

Received_data received_data;

int ch1_value = 0;
int ch2_value = 0;
int ch3_value = 0;
int ch4_value = 0;
int ch5_value = 0;
int ch6_value = 0;
int ch7_value = 0;



void setup() {
  Serial.begin(115200);

  // Mínimo a configurar para que funcione:
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);                // RF24_PA_MAX is default.
  radio.setPayloadSize(sizeof(Received_data));  // float datatype occupies 4 bytes
  radio.openReadingPipe(1, address[0]);         // using pipe 1
  radio.startListening();                       // put radio in RX mode


  pinMode(rele1Motor1, OUTPUT);
  pinMode(rele2Motor1, OUTPUT);
  pinMode(rele3Motor2, OUTPUT);
  pinMode(rele4Motor2, OUTPUT);

  digitalWrite(rele1Motor1, HIGH);
  digitalWrite(rele2Motor1, HIGH);
  digitalWrite(rele3Motor2, HIGH);
  digitalWrite(rele4Motor2, HIGH);

  reset_the_Data();
}

void loop() {
  //Receive the radio data
  // reset_the_Data();

  receive_the_data();
  // Serial.println(millis());
  // delay(3000);
}



//We create the function that will read the data each certain time
void receive_the_data() {
  if (radio.available()) {
    radio.read(&received_data, sizeof(Received_data));
    lastRecvTime = millis();  //Here we receive the data
    Serial.println("Datos recibidos.");
    ch1_value = received_data.ch1;
    ch2_value = received_data.ch2;
    ch3_value = received_data.ch3;
    ch4_value = received_data.ch4;
    ch5_value = received_data.ch5;
    ch6_value = received_data.ch6;
    ch7_value = received_data.ch7;

    Serial.print("ch1_value: ");
    Serial.println(ch1_value);
    Serial.print("ch2_value: ");
    Serial.println(ch2_value);
    Serial.print("ch3_value: ");
    Serial.println(ch3_value);
    Serial.print("ch4_value: ");
    Serial.println(ch4_value);
    Serial.print("ch5_value: ");
    Serial.println(ch5_value);
    Serial.print("ch6_value: ");
    Serial.println(ch6_value);
    Serial.print("ch7_value: ");
    Serial.println(ch7_value);

    analogWrite(m1PWM, ch1_value);
    analogWrite(m2PWM, ch2_value);

    newStateRele1y2 = ch3_value != 0;
    if (newStateRele1y2) {
      if (!(newStateRele1y2 == stateRele1y2)) {
        digitalWrite(rele1Motor1, LOW);
        digitalWrite(rele2Motor1, LOW);
        stateRele1y2 = newStateRele1y2;
        Serial.print("1");
      }
    } else {
      if (!(newStateRele1y2 == stateRele1y2)) {
        digitalWrite(rele1Motor1, HIGH);
        digitalWrite(rele2Motor1, HIGH);
        stateRele1y2 = newStateRele1y2;
        Serial.print("2");
      }
    }

    newStateRele3y4 = ch4_value != 0;
    if (newStateRele3y4) {
      if (!(newStateRele3y4 == stateRele3y4)) {
        digitalWrite(rele3Motor2, LOW);
        digitalWrite(rele4Motor2, LOW);
        stateRele3y4 = newStateRele3y4;
        Serial.print("3");
      }
    } else {
      if (!(newStateRele3y4 == stateRele3y4)) {
        digitalWrite(rele3Motor2, HIGH);
        digitalWrite(rele4Motor2, HIGH);
        stateRele3y4 = newStateRele3y4;
        Serial.print("4");
      }
    }
  }
    // reset_the_Data();
  // Serial.println("receive_the_data");
  // radio.printDetails();
}

void reset_the_Data() {
  received_data.ch1 = 0;
  received_data.ch2 = 0;
  received_data.ch3 = 0;
  received_data.ch4 = 0;
  received_data.ch5 = 0;
  received_data.ch6 = 0;
  received_data.ch7 = 0;

  // digitalWrite(rele1Motor1, HIGH);
  // digitalWrite(rele2Motor1, HIGH);
  // digitalWrite(rele3Motor2, HIGH);
  // digitalWrite(rele4Motor2, HIGH);
}