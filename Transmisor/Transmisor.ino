#include <SPI.h>
#include <nRF24L01.h>  //Remember to isntall this bibrry: http://www.electronoobs.com/engarduino_NRF24_lib.php
#include <RF24.h>

// Joystick
#define x 13
#define y 12
#define bot 14

// // OUTPUT Signals
// #define izq 27
// #define der 26
// #define up 25
// #define down 33
// #define boom 32

// const byte direction = '92654'; //Remember that this code should be the same for the receiver
uint8_t address[6] = { "mdca0" };

RF24 radio(4, 5);  //Set CE and CSN pins

int errorx = -800, errory = -800;

// graduarJoystick();


// The sizeof this struct should not exceed 32 bytes
struct Data_to_be_sent {
  byte ch1;  // motor 1 - PWM - 0-255
  byte ch2;  // motor 2 - PWM - 0-255
  byte ch3;  // Rele 1 y 2 - Motor1
  byte ch4;  // Rele 3 y 4 - Motor2
  byte ch5;  //
  byte ch6;  //
  byte ch7;  //
};

//Create a variable with the structure above and name it sent_data
Data_to_be_sent sent_data;


void setup() {
  Serial.begin(115200);
  radio.begin();

  analogWrite(bot, 255);

  radio.setPALevel(RF24_PA_LOW);  // RF24_PA_MAX is default.
  // Mínimo a configurar para que funcione:
  radio.setPayloadSize(sizeof(Data_to_be_sent));  // float datatype occupies 4 bytes
  radio.openWritingPipe(address[0]);              // always uses pipe 0
  radio.stopListening();                          // put radio in TX mode

  pinMode(x, INPUT);
  pinMode(y, INPUT);
  // pinMode(bot, INPUT);


  //Reset each channel value
  reset_the_Data();
  delay(1000);
}

void loop() {
  // Serial.println(millis());
  controlMotor();

  // if ((sent_data.ch1 != 0) || (sent_data.ch2 != 0)) {

  unsigned long start_timer = micros();                            // start the timer
  bool report = radio.write(&sent_data, sizeof(Data_to_be_sent));  // transmit & save the report
  unsigned long end_timer = micros();                              // end the timer
  if (report) {
    Serial.print(F("Transmission successful! "));  // payload was delivered
    Serial.print(F("Time to transmit = "));
    Serial.print(end_timer - start_timer);  // print the timer result

    Serial.print("sent_data.ch1: ");
    Serial.println(sent_data.ch1);
    Serial.print("sent_data.ch2: ");
    Serial.println(sent_data.ch2);
    Serial.print("sent_data.ch3: ");
    Serial.println(sent_data.ch3);
    Serial.print("sent_data.ch4: ");
    Serial.println(sent_data.ch4);
    Serial.print("sent_data.ch5: ");
    Serial.println(sent_data.ch5);
    Serial.print("sent_data.ch6: ");
    Serial.println(sent_data.ch6);
    Serial.print("sent_data.ch7: ");
    Serial.println(sent_data.ch7);
  } else {
    Serial.println(F("Transmission failed or timed out"));  // payload was not delivered
  }
  // }

  // to make this example readable in the serial monitor
  // delay(100);  // slow transmissions down by 1 second
}

void controlMotor() {
  // Lectura de datos:
  int x_value = analogRead(x);      // 0 - 4095 (Izq - Der)
  int y_value = analogRead(y);      // 0 - 4095 (Up - Down)
  int bot_value = analogRead(bot);  // 0 = accionado ; 4095 Normal state

  Serial.print("x_value: ");
  Serial.println(x_value);
  Serial.print("y_value: ");
  Serial.println(y_value);

  // if (bot_value == 0) {
  //   graduarJoystick();
  // }

  // Serial.print("x_value: ");
  // Serial.println(x_value);

  // Normalizando datos:
  x_value = map(x_value, 0, 4095, 0, 1000);
  y_value = map(y_value, 0, 4095, 0, 1000);
  // if (errorx < 0)
  //   x_value = map(x_value, 0 - errorx, 4095, 0, 1000);
  // else
  //   x_value = map(x_value, 0, 4095 - errorx, 0, 1000);

  // if (errory < 0)
  //   y_value = map(y_value, 0 - errory, 4095, 0, 1000);
  // else
  //   y_value = map(y_value, 0, 4095 - errory, 0, 1000);

  Serial.print("x_value: ");
  Serial.println(x_value);
  Serial.print("y_value: ");
  Serial.println(y_value);
  Serial.print("bot_value: ");
  Serial.println(bot_value);


  if (((x_value < 690) or (x_value > 760))) {
    if (x_value > 760) {  // Der
      sent_data.ch1 = map(x_value, 740, 1000, 50, 255);
      sent_data.ch2 = map(x_value, 740, 1000, 50, 255);
      sent_data.ch3 = 0;
      sent_data.ch4 = 1;
    }
    if (x_value < 690) {  // Izq
      sent_data.ch1 = map(-x_value + 690, 0, 690, 50, 255);
      sent_data.ch2 = map(-x_value + 690, 0, 690, 50, 255);
      sent_data.ch3 = 1;
      sent_data.ch4 = 0;
    }
  } else if (((y_value < 680) or (y_value > 760))) {
    if (y_value < 680) {  // Arriba - Avanzar
      sent_data.ch1 = map(-y_value + 680, 0, 680, 50, 255);
      sent_data.ch2 = map(-y_value + 680, 0, 680, 50, 255);
      sent_data.ch3 = 0;
      sent_data.ch4 = 0;
    }
    if (y_value > 760) {  // Abajo - Retroceder
      sent_data.ch1 = map(y_value, 730, 1000, 50, 255);
      sent_data.ch2 = map(y_value, 730, 1000, 50, 255);
      sent_data.ch3 = 1;
      sent_data.ch4 = 1;
    }
  } else {
    reset_the_Data();
  }

  // Serial.print("sent_data.ch1: ");
  // Serial.println(sent_data.ch1);
  // Serial.print("sent_data.ch2: ");
  // Serial.println(sent_data.ch2);
  // Serial.print("sent_data.ch3: ");
  // Serial.println(sent_data.ch3);
  // Serial.print("sent_data.ch4: ");
  // Serial.println(sent_data.ch4);
  // Serial.print("sent_data.ch5: ");
  // Serial.println(sent_data.ch5);
  // Serial.print("sent_data.ch6: ");
  // Serial.println(sent_data.ch6);
  // Serial.print("sent_data.ch7: ");
  // Serial.println(sent_data.ch7);
}


void graduarJoystick() {
  delay(1000);
  int sumx = 0, sumy = 0, conteoMuestra = 1;
  Serial.print("Graduando Joystick.");
  while (conteoMuestra < 30) {

    sumx += analogRead(x);
    sumy += analogRead(y);

    Serial.print(".");
    Serial.print(String(conteoMuestra) + "sumx: ");
    Serial.println(sumx);
    Serial.print(String(conteoMuestra) + "sumy: ");
    Serial.println(sumy);
    conteoMuestra++;
    delay(100);
  }
  errorx = (sumx / (conteoMuestra - 1));
  errory = (sumy / (conteoMuestra - 1));

  if (errorx > 2048) {
    errorx = -1 * (errorx - 2048);
  } else {
    errorx = errorx - 2048;
  }

  if (errory > 2048) {
    errory = -1 * (errory - 2048);
  } else {
    errory = (errory - 2048);
  }
  Serial.print("  Completado. Error x: ");
  Serial.print(errorx);
  Serial.print(", error y: ");
  Serial.println(errory);
}

void reset_the_Data() {
  sent_data.ch1 = 0;
  sent_data.ch2 = 0;
  sent_data.ch3 = 0;
  sent_data.ch4 = 0;
  sent_data.ch5 = 0;
  sent_data.ch6 = 0;
  sent_data.ch7 = 0;
}