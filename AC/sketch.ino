#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <SPI.h>
#include <DHT.h>
#include <WiFi.h>
#include <HTTPClient.h>

// WiFi e ThingSpeak
const char* ssid = "Wokwi-GUEST";
const char* senha = "";
const char* server = "http://api.thingspeak.com/update";
const char* apiKey = "GGO2WRLPFLMHNTN9";

// Pinos dos sensores e display
#define DHTPIN 15
#define DHTTYPE DHT22
#define PIRPIN 13
#define LEDPIN 14

#define TFT_CS   5
#define TFT_RST  4
#define TFT_DC   2

DHT dht(DHTPIN, DHTTYPE);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

bool acLigado = false;
unsigned long lastUpdate = 0;
const long updateInterval = 2000;

void setup() {
  Serial.begin(115200);
  dht.begin();

  pinMode(PIRPIN, INPUT);
  pinMode(LEDPIN, OUTPUT);
  digitalWrite(LEDPIN, LOW);

  tft.begin();
  tft.setRotation(1);
  tft.fillScreen(ILI9341_BLACK);
  drawInterface();

  // Conectar ao Wi-Fi
  WiFi.begin(ssid, senha);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - lastUpdate >= updateInterval) {
    lastUpdate = currentMillis;

    float temperatura = dht.readTemperature();
    float umidade = dht.readHumidity();
    int presenca = digitalRead(PIRPIN);

    updateDisplay(temperatura, umidade, presenca);
    controlAC(temperatura, presenca);
    sendToThingSpeak(temperatura, umidade, presenca);
  }
}

void drawInterface() {
  tft.fillScreen(ILI9341_BLACK);
  tft.setTextColor(ILI9341_WHITE);
  tft.setTextSize(2);
  tft.setCursor(80, 10);
  tft.print("CONTROLE DE AC");

  tft.drawFastHLine(0, 40, 240, ILI9341_WHITE);

  tft.setTextSize(1);
  tft.setCursor(10, 60);  tft.print("Temperatura:");
  tft.setCursor(10, 100); tft.print("Umidade:");
  tft.setCursor(10, 140); tft.print("Presenca:");
  tft.setCursor(10, 180); tft.print("Status AC:");
}

void updateDisplay(float temp, float umid, int pres) {
  tft.fillRect(120, 60, 100, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);
  tft.setTextSize(2);
  tft.setCursor(120, 60);
  tft.print(temp, 1); tft.print(" C");

  tft.fillRect(120, 100, 100, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_CYAN);
  tft.setCursor(120, 100);
  tft.print(umid, 1); tft.print(" %");

  tft.fillRect(120, 140, 100, 20, ILI9341_BLACK);
  tft.setTextColor(pres ? ILI9341_GREEN : ILI9341_RED);
  tft.setCursor(120, 140);
  tft.print(pres ? "Detectada" : "Ausente");
}

void controlAC(float temperatura, int presenca) {
  bool deveLigar = false;

  if (temperatura >= 24.0) {
    deveLigar = true;
    Serial.print("AC Ligado (Temperatura alta)");
  } else if (temperatura >= 18.0 && presenca) {
    deveLigar = true;
    Serial.print("AC Ligado (Temperatura moderada com presença)");
  }

  if (deveLigar && !acLigado) {
    digitalWrite(LEDPIN, HIGH);
    acLigado = true;
    updateACStatus(true);
    Serial.println(" - AC LIGADO");
  } else if (!deveLigar && acLigado) {
    digitalWrite(LEDPIN, LOW);
    acLigado = false;
    updateACStatus(false);
    Serial.println("AC Desligado!");
  }
}

void updateACStatus(bool status) {
  tft.fillRect(120, 180, 100, 20, ILI9341_BLACK);
  tft.setTextColor(status ? ILI9341_GREEN : ILI9341_RED);
  tft.setTextSize(2);
  tft.setCursor(120, 180);
  tft.print(status ? "LIGADO" : "DESLIGADO");

  tft.fillRect(230, 180, 60, 20, status ? ILI9341_GREEN : ILI9341_RED);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(235, 180);
  tft.print(status ? "ON" : "OFF");
}

void sendToThingSpeak(float temp, float hum, int presenca) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    int statusAC = acLigado ? 1 : 0;
    String statusMsg = acLigado ? "LIGADO" : "DESLIGADO";

    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(temp) +
                 "&field2=" + String(hum) +
                 "&field3=" + String(presenca) +
                 "&field4=" + String(statusAC) +
                 "&field5=" + statusMsg;

    Serial.print("Enviando para ThingSpeak: ");
    Serial.println(url);

    http.begin(url);
    int httpResponseCode = http.GET();

    if (httpResponseCode == 200) {
      Serial.println("Dados enviados com sucesso!");
    } else {
      Serial.print("Erro no envio: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi desconectado! Reconectando...");
    WiFi.begin(ssid, senha);
  }
}

