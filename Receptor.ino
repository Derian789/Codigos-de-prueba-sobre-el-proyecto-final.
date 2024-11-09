#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <SoftwareSerial.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4
#define CLK_PIN   6
#define DATA_PIN  4
#define CS_PIN    5

MD_Parola display = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

SoftwareSerial mySerial(3, 2); // Tx, Rx
#define BUTTON_PIN 7

#include "funciones.h"

void setup() {
  Serial.begin(9600);
  mySerial.begin(9600);

  display.begin();
  display.setIntensity(5);
  display.displayClear();

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("Inicializando...");
  delay(1000);

  mySerial.println("AT");
  updateSerial();

  mySerial.println("AT+CMGF=1"); // Configurar en modo TEXTO para SMS
  updateSerial();

  mySerial.println("AT+CNMI=2,2,0,0,0");
  updateSerial();
}

void loop() {
  // Leer mensajes entrantes y mostrarlos en el Monitor Serial y la matriz LED
  if (mySerial.available()) {
    String smsContent = readIncomingMessage();
    Serial.println("Mensaje recibido: " + smsContent);
    displayMessage(smsContent);
  }

  // Enviar mensaje si el pulsador es presionado
  if (digitalRead(BUTTON_PIN) == LOW) {
    sendSMS();
    delay(1000);
  }
}
