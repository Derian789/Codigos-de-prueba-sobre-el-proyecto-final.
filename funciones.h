
void updateSerial() {
  delay(500);
  while (Serial.available()) {
    mySerial.write(Serial.read());
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());
  }
}
//funcion para leer mensajes
String readIncomingMessage() {
  String message = "";
  bool messageStarted = false;

  while (mySerial.available()) {
    char c = (char)mySerial.read();

    if (c == '\n' && !messageStarted) {
      messageStarted = true; // Inicia la captura del contenido real
      continue; // Omitir esta línea en blanco
    }

    if (messageStarted) {
      message += c; // Agregar caracteres al mensaje
    }
  }
  message.trim(); // Limpiar espacios y líneas en blanco
  return message;
}

// Función para enviar un mensaje SMS
void sendSMS() {
  mySerial.println("AT+CMGS=\"+573007283103\"");
  updateSerial();
  mySerial.print("Hola mundo!!!");
  updateSerial();
  mySerial.write(26); // Código ASCII para Ctrl+Z
  Serial.println("Mensaje enviado.");
}

// Función para mostrar el mensaje en la matriz LED
void displayMessage(String message) {
  if (message.length() > 44) {
    char msgTrue[11]; // Espacio para 10 caracteres 
    strncpy(msgTrue, &message[44], 10); // Copiar 10 caracteres desde la posición 44
    msgTrue[10] = '\0'; // Asegurar el terminador nulo

    display.displayText(msgTrue, PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT); // Mostrar y desplazar el texto
    while (!display.displayAnimate()) {
      // Esperar a que termine la animación de desplazamiento
    }
  }
}
