#define BLYNK_TEMPLATE_ID "TMPL27Al1vQEG"
#define BLYNK_TEMPLATE_NAME "XoEnchente"
#define BLYNK_AUTH_TOKEN "NJNiFINbSBZu8zrqO1YNi3Pl4GV3XVSC"
#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define echoPin 2
#define trigPin 15
#define LEDPin 13


// Credenciais da Wi-Fi.
char ssid[] = "";
char pass[] = "";

BlynkTimer timer;

BLYNK_CONNECTED() {
  printf("Conectado à plataforma Blynk.");
}

int statusDoLED = 0;

float pegarDistanciaAteAAgua() {
  float velocidadeDoSom = 0.0344;
  long tempoDeViagemTotalDoPulso;
  int distancia;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  tempoDeViagemTotalDoPulso = pulseIn(echoPin, HIGH);
  distancia = (tempoDeViagemTotalDoPulso / 2) * velocidadeDoSom;

  return distancia;
}

void piscarOLEDEEnviarParaOBlynk () {
  statusDoLED = !statusDoLED;
  digitalWrite(LEDPin, statusDoLED);

  Blynk.virtualWrite(V1, statusDoLED);
}

void enviarDistanciaParaOBlynk() {
  Blynk.virtualWrite(V0, pegarDistanciaAteAAgua());
}

void setup() {
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT);

  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  timer.setInterval(1000L, enviarDistanciaParaOBlynk);
  timer.setInterval(1000L, piscarOLEDEEnviarParaOBlynk);
}

void loop() {
  Blynk.run();
  timer.run();
}
