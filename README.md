# 🌡️ Controle de Ar-Condicionado com ESP32 + ThingSpeak

Projeto de automação com ESP32 que realiza a leitura de sensores de **temperatura**, **umidade** e **presença**, controla o status do **ar-condicionado** e envia os dados para o **ThingSpeak**, além de exibir as informações em um display LCD/I2C ou TFT.

## 📷 Imagens do Projeto

### Interface no ThingSpeak

> ![Image](https://github.com/user-attachments/assets/e4c093b6-a6bd-4d96-b505-a2e91e74f013)

### Circuito no Simulador

![Image](https://github.com/user-attachments/assets/1f89c99c-f418-444b-97fb-35c14871f981)

### Display TFT com status do sistema

![Image](https://github.com/user-attachments/assets/c8b7ceac-8a2e-4a15-b423-afc21710da95)

---

## ⚙️ Funcionalidades

- 📡 Conexão com WiFi.
- 🌡️ Leitura de temperatura e umidade (sensor DHT22).
- 🚶 Detecção de presença (sensor PIR).
- 🆒 Controle do ar-condicionado com LED representando o estado.
- 📈 Envio de dados para o ThingSpeak.
- 🖥️ Exibição local das leituras e status no display LCD ou TFT.

---

## 📶 ThingSpeak - Visualização dos dados

Os dados são enviados para os seguintes campos no ThingSpeak:

- **Field 1**: Temperatura do ambiente.
- **Field 2**: Umidade relativa do ar.
- **Field 3**: Presença detectada.
- **Field 4**: Status do ar-condicionado (ligado/desligado).

---

## 🛠️ Componentes usados

- ESP32 DevKit
- Sensor DHT22
- Sensor PIR
- Display LCD 16x2 com I2C **ou** Display TFT ILI9341
- LED para indicar o status do ar-condicionado
- Jumpers e protoboard

---

## 🧠 Lógica do Projeto

1. Conecta à rede WiFi.
2. Lê temperatura, umidade e presença.
3. Controla o status do AC (simulado com LED) com base nas leituras.
4. Mostra dados em tempo real no display.
5. Envia os dados a cada 15 segundos para a plataforma ThingSpeak.

---

## 📦 Bibliotecas Utilizadas

- `WiFi.h`  
- `HTTPClient.h`  
- `LiquidCrystal_I2C.h`  
- `Adafruit_GFX.h` (para TFT, se usado)  
- `Adafruit_ILI9341.h` (para TFT, se usado)  
- `DHT.h`

---

## 📋 Como Usar

1. Instale as bibliotecas necessárias via Gerenciador de Bibliotecas da IDE Arduino.
2. Configure seu WiFi e chave da API do ThingSpeak no código.
3. Compile e envie o código para o ESP32.
4. Acompanhe os dados no display e no ThingSpeak.

---

## 📊 Exemplo dos Gráficos

| Temperatura | Umidade | Presença | Status AC |
|-------------|---------|----------|-----------|
| > ![Image](https://github.com/user-attachments/assets/c4e3abd2-f32f-4f34-b19d-6797ed9b9cfc) | > ![Image](https://github.com/user-attachments/assets/3fea9865-0364-42e5-b35c-717d88572673) |> ![Image](https://github.com/user-attachments/assets/53779008-f356-4a4f-a84c-316af0e7874f) | > ![Image](https://github.com/user-attachments/assets/8192d13e-f4de-41ce-8409-2b9959e03d7c) |

---

## ✅ Melhorias Futuras

- Utilizar gauges (medidores) no ThingSpeak.
- Adicionar controle remoto via web.
- Integração com assistentes virtuais (Google Assistant, Alexa).
- Registro em banco de dados externo.

---

## 👨‍💻 Autor

**Evandro**  
Jaraguá do Sul - SC  
Estudante de Desenvolvimento de Sistemas  


