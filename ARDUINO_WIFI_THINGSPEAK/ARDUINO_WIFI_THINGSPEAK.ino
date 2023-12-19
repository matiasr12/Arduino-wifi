#include <ESP8266WiFi.h>
#include <Servo.h>
#include <ThingSpeak.h>

const char* ssid = "LAPTOP-A";
const char* password = "12345678";
const char* thingSpeakApiKey = "UAFGYBB4R9CGW7QB";
const int channelNumber = 2384362;  // Reemplaza con el número de tu canal ThingSpeak
const int fieldNumber = 1;         // Reemplaza con el número de campo en tu canal
const int pinReceptor = 2;

Servo servoMotor;
int sensorVal;
boolean leerSensor = true;

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Conéctate a la red WiFi
  WiFi.begin(ssid, password);

  Serial.print("Conectando");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado al WiFi");
  Serial.println("Dirección IP: " + WiFi.localIP().toString());

  servoMotor.attach(0);
  servoMotor.write(150);

  ThingSpeak.begin(client);
}

void loop() {
  sensorVal = digitalRead(pinReceptor);

  Serial.println("Digital:");
  Serial.println(sensorVal);

  delay(500);

  if (sensorVal == LOW && leerSensor) {
    if (leerSensor) {
      leerSensor = false;
    }
    servoMotor.write(0);
    delay(3000);
    servoMotor.write(180);
    delay(3000);

    // Envía datos a ThingSpeak cuando el servoMotor se activa
    ThingSpeak.writeField(channelNumber, fieldNumber, 1, thingSpeakApiKey);
  } else if (sensorVal == HIGH) {
    leerSensor = true;
  }
}
