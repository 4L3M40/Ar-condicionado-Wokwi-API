#include <Wire.h>                    // Biblioteca para comunicação I2C
#include <Adafruit_GFX.h>            // Biblioteca gráfica para displays
#include <Adafruit_ILI9341.h>        // Biblioteca específica para o display TFT ILI9341
#include <SPI.h>                     // Biblioteca para comunicação SPI
#include <DHT.h>                     // Biblioteca para sensor de temperatura e umidade DHT
#include <WiFi.h>                    // Biblioteca para conexão WiFi no ESP32
#include <HTTPClient.h>              // Biblioteca para fazer requisições HTTP

// Configurações de WiFi e ThingSpeak
const char* ssid = "Wokwi-GUEST";    // Nome da rede WiFi (Wokwi simula esta rede)
const char* senha = "";              // Senha da rede WiFi (no caso, vazio)
const char* server = "http://api.thingspeak.com/update";  // URL da API do ThingSpeak
const char* apiKey = "GGO2WRLPFLMHNTN9";                  // Chave de API para enviar dados ao ThingSpeak

// Definição dos pinos utilizados
#define DHTPIN 15                   // Pino conectado ao sensor DHT22
#define DHTTYPE DHT22               // Tipo do sensor DHT (DHT22)
#define PIRPIN 13                   // Pino conectado ao sensor de presença PIR
#define LEDPIN 14                   // Pino conectado ao LED que indica status do ar-condicionado

#define TFT_CS   5                  // Pino Chip Select do display TFT
#define TFT_RST  4                  // Pino Reset do display TFT
#define TFT_DC   2                  // Pino Data/Command do display TFT

// Inicialização do sensor DHT e do display TFT
DHT dht(DHTPIN, DHTTYPE);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Variáveis de controle
bool acLigado = false;               // Estado atual do ar-condicionado (ligado ou desligado)
unsigned long lastUpdate = 0;        // Armazena o tempo da última atualização
const long updateInterval = 2000;    // Intervalo de atualização em milissegundos (2 segundos)

void setup() {
  Serial.begin(115200);              // Inicializa comunicação serial para debug
  dht.begin();                      // Inicializa sensor DHT22

  pinMode(PIRPIN, INPUT);           // Define o pino do sensor PIR como entrada
  pinMode(LEDPIN, OUTPUT);          // Define o pino do LED como saída
  digitalWrite(LEDPIN, LOW);        // Inicialmente, LED desligado

  tft.begin();                      // Inicializa display TFT
  tft.setRotation(1);               // Define orientação do display (paisagem)
  tft.fillScreen(ILI9341_BLACK);   // Limpa a tela com fundo preto
  drawInterface();                  // Desenha a interface estática inicial

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, senha);
  Serial.print("Conectando ao WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");              // Mostra pontos enquanto tenta conectar
  }
  Serial.println("\nWiFi conectado!");
}

void loop() {
  unsigned long currentMillis = millis();  // Captura o tempo atual

  // Verifica se passou o intervalo de atualização para executar as leituras e envio
  if (currentMillis - lastUpdate >= updateInterval) {
    lastUpdate = currentMillis;             // Atualiza o tempo da última execução

    float temperatura = dht.readTemperature();  // Lê temperatura do sensor DHT22
    float umidade = dht.readHumidity();           // Lê umidade do sensor DHT22
    int presenca = digitalRead(PIRPIN);           // Lê estado do sensor de presença (0 ou 1)

    updateDisplay(temperatura, umidade, presenca);  // Atualiza os dados no display TFT
    controlAC(temperatura, presenca);                // Controla o status do ar-condicionado
    sendToThingSpeak(temperatura, umidade, presenca);// Envia dados para o ThingSpeak
  }
}

// Desenha a interface estática do display TFT com os títulos dos dados
void drawInterface() {
  tft.fillScreen(ILI9341_BLACK);               // Fundo preto
  tft.setTextColor(ILI9341_WHITE);             // Texto branco
  tft.setTextSize(2);                           // Tamanho da fonte maior
  tft.setCursor(80, 10);                        // Posição do texto
  tft.print("CONTROLE DE AC");                  // Título

  tft.drawFastHLine(0, 40, 240, ILI9341_WHITE); // Linha horizontal para separar título

  tft.setTextSize(1);                           // Fonte menor para dados
  tft.setCursor(10, 60);  tft.print("Temperatura:");
  tft.setCursor(10, 100); tft.print("Umidade:");
  tft.setCursor(10, 140); tft.print("Presenca:");
  tft.setCursor(10, 180); tft.print("Status AC:");
}

// Atualiza os valores dinâmicos do display com os dados atuais
void updateDisplay(float temp, float umid, int pres) {
  // Limpa área da temperatura antes de atualizar
  tft.fillRect(120, 60, 100, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_YELLOW);           // Cor amarela para temperatura
  tft.setTextSize(2);
  tft.setCursor(120, 60);
  tft.print(temp, 1);                          // Imprime temperatura com 1 casa decimal
  tft.print(" C");

  // Limpa área da umidade e atualiza
  tft.fillRect(120, 100, 100, 20, ILI9341_BLACK);
  tft.setTextColor(ILI9341_CYAN);             // Cor ciano para umidade
  tft.setCursor(120, 100);
  tft.print(umid, 1);                          // Imprime umidade com 1 casa decimal
  tft.print(" %");

  // Limpa área da presença e atualiza
  tft.fillRect(120, 140, 100, 20, ILI9341_BLACK);
  tft.setTextColor(pres ? ILI9341_GREEN : ILI9341_RED);  // Verde se presença detectada, vermelho se ausente
  tft.setCursor(120, 140);
  tft.print(pres ? "Detectada" : "Ausente");
}

// Controla o estado do ar-condicionado com base na temperatura e presença
void controlAC(float temperatura, int presenca) {
  bool deveLigar = false;

  // Lógica para ligar o AC:
  // Se temperatura >= 24°C, liga direto
  if (temperatura >= 24.0) {
    deveLigar = true;
    Serial.print("AC Ligado (Temperatura alta)");
  }
  // Se temperatura entre 18°C e 24°C e presença detectada, também liga
  else if (temperatura >= 18.0 && presenca) {
    deveLigar = true;
    Serial.print("AC Ligado (Temperatura moderada com presença)");
  }

  // Se deve ligar e ainda está desligado, liga o AC e acende o LED
  if (deveLigar && !acLigado) {
    digitalWrite(LEDPIN, HIGH);
    acLigado = true;
    updateACStatus(true);           // Atualiza status no display
    Serial.println(" - AC LIGADO");
  }
  // Se não deve ligar e AC está ligado, desliga o AC e apaga o LED
  else if (!deveLigar && acLigado) {
    digitalWrite(LEDPIN, LOW);
    acLigado = false;
    updateACStatus(false);
    Serial.println("AC Desligado!");
  }
}

// Atualiza a área do display que mostra o status do ar-condicionado (ligado/desligado)
void updateACStatus(bool status) {
  tft.fillRect(120, 180, 100, 20, ILI9341_BLACK);          // Limpa área
  tft.setTextColor(status ? ILI9341_GREEN : ILI9341_RED);  // Verde para ligado, vermelho para desligado
  tft.setTextSize(2);
  tft.setCursor(120, 180);
  tft.print(status ? "LIGADO" : "DESLIGADO");

  // Indicador ON/OFF colorido
  tft.fillRect(230, 180, 60, 20, status ? ILI9341_GREEN : ILI9341_RED);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(235, 180);
  tft.print(status ? "ON" : "OFF");
}

// Envia os dados coletados para o ThingSpeak via requisição HTTP GET
void sendToThingSpeak(float temp, float hum, int presenca) {
  if (WiFi.status() == WL_CONNECTED) {       // Verifica se está conectado ao WiFi
    HTTPClient http;

    int statusAC = acLigado ? 1 : 0;         // Representa o status do AC em número (1=ligado, 0=desligado)
    String statusMsg = acLigado ? "LIGADO" : "DESLIGADO";

    // Monta a URL com os parâmetros para enviar ao ThingSpeak (5 campos)
    String url = String(server) + "?api_key=" + apiKey +
                 "&field1=" + String(temp) +
                 "&field2=" + String(hum) +
                 "&field3=" + String(presenca) +
                 "&field4=" + String(statusAC) +
                 "&field5=" + statusMsg;

    Serial.print("Enviando para ThingSpeak: ");
    Serial.println(url);

    http.begin(url);                         // Inicia conexão HTTP com a URL
    int httpResponseCode = http.GET();      // Envia requisição GET

    if (httpResponseCode == 200) {          // Verifica se envio foi bem-sucedido
      Serial.println("Dados enviados com sucesso!");
    } else {
      Serial.print("Erro no envio: ");
      Serial.println(httpResponseCode);     // Exibe código de erro
    }

    http.end();                             // Encerra conexão HTTP
  } else {
    Serial.println("WiFi desconectado! Reconectando...");
    WiFi.begin(ssid, senha);                 // Tenta reconectar o Wi-Fi
  }
}
