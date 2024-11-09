#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus neo6m;
SoftwareSerial gpsSerial(7, 8);
SoftwareSerial mySerial(3, 2);

double calcularDistancia(double lat1, double lon1, double lat2, double lon2) {
  const double R = 6371000; // Radio de la Tierra en metros
  double dLat = radians(lat2 - lat1);
  double dLon = radians(lon2 - lon1);

  lat1 = radians(lat1);
  lat2 = radians(lat2);

  double a = sin(dLat / 2) * sin(dLat / 2) +
             sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
  double c = 2 * atan2(sqrt(a), sqrt(1 - a));
  return R * c; // Retorna la distancia en metros
}

double convertirStringADouble(String valor) {
  return valor.toDouble();
}

double calcularTiempoEstimado(double lat1, double lon1, double lat2, double lon2, double velocidad) {
  double distancia = calcularDistancia(lat1, lon1, lat2, lon2); // Distancia en metros
  double tiempoEnSegundos = distancia / velocidad; // Tiempo en segundos
  double tiempoEnMinutos = tiempoEnSegundos / 60; // Tiempo en minutos
  return tiempoEnMinutos; // Retorna el tiempo en minutos
}

// Función para actualizar la comunicación entre Serial y mySerial
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}

// Función para enviar un mensaje SMS
void sendSMS(String message) {
  mySerial.println("AT+CMGS=\"+573007283103\"");
  updateSerial();
  mySerial.print(message);
  updateSerial();
  mySerial.write(26);
  Serial.println("Mensaje enviado.");
  return message;
}

String punto1_lat = "4.55634";
String punto1_lon = "-75.65683";

int pulsaPin = 4;
int estadoPull = 0;

#include "fuctions.h"

void setup() {
  // put your setup code here, to run once:
  gpsSerial.begin(9600);
  Serial.begin(9600);
  pinMode(pulsaPin, INPUT_PULLUP);

}

void loop() {
  // put your main code here, to run repeatedly:
  while (gpsSerial.available() > 0) {
    neo6m.encode(gpsSerial.read());

    if (neo6m.location.isUpdated()) {
      double gps_lat = neo6m.location.lat();
      double gps_lon = neo6m.location.lng();

      double punto1_lat_d = convertirStringADouble(punto1_lat);
      double punto1_lon_d = convertirStringADouble(punto1_lon);

      double distancia = calcularDistancia(gps_lat, gps_lon, punto1_lat_d, punto1_lon_d);

      double velocidad = 50.0;

      double tiempoMin = calcularTiempoEstimado(gps_lat, gps_lon, punto1_lat_d, punto1_lon_d, velocidad);

      Serial.println("Tiempo estimado: ");
      Serial.println(tiempoMin);

      estadoPull = digitalRead(pulsaPin);

      if (estadoPull == LOW) {
        sendSMS(String(tiempoMin));
        delay(1000);
      }
    }

  }
}
