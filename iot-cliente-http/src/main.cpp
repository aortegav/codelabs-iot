#include <Arduino.h>
#include "libwifi.h"
#include <WiFi.h>

#define LED 2

const char *ssid = "Patio";
const char *password = "al1968val*";

const char *host = "dweet.io";
const int port = 80;
const char *thing_name = "sensor01";	

void setup()
{
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  Serial.println("Iniciando cliente http");
  conectarWifi(ssid, password);
}

void loop()
{
  WiFiClient cliente;

  if (!cliente.connect(host, port))
  {
    delay(2000);
    return;
  }

  int temperatura = random(20, 27); // Valores entre 20 y 26
  int humedad = random(30, 41);     // Valores entre 30 y 40

  String url = "/dweet/for/" + String(thing_name) + "?temperatura=" + String(temperatura) + "&&humedad=" + String(humedad);

  cliente.print("GET " + url + " HTTP/1.1\r\nHost: " + String(host) + "\r\nConnection: close\r\n\r\n");
  //cliente.print("GET /dweet/for/" + String(thing_name) + "?temperatura=" + String(temperatura) + "&&humedad=" + String(humedad) + " HTTP/1.1\r\nHost:" + String(host) + "\r\nConnection: close\r\n\r\n");

  // Agregamos algun tiempo de espera para recibir los primeros caracteres del servidor
  unsigned long tiempo = millis();
  while (cliente.available() == 0)
  {
    if (millis() - tiempo > 5000)
    {
      Serial.println("Tiempo de espera agotado");
      cliente.stop();
      return;
    }
  }

  while (cliente.available())
  {

    String linea = cliente.readStringUntil('\r');
    Serial.println(linea);
  }

  Serial.println("Datos enviados correctamente");
  cliente.stop();
  delay(3000);
}
