# 🌡️ Controle de Ar-Condicionado com ESP32 + ThingSpeak

Projeto de automação com ESP32 que realiza a leitura de sensores de **temperatura**, **umidade** e **presença**, controla o status do **ar-condicionado** e envia os dados para o **ThingSpeak**, além de exibir as informações em um display LCD/I2C ou TFT.

## 📷 Imagens do Projeto

### Interface no ThingSpeak

![Gráficos ThingSpeak](./Captura%20de%20tela%202025-05-22%20202135.png)

### Circuito no Simulador

![Circuito completo no simulador](./Captura%20de%20tela%202025-05-22%20202219.png)

### Display TFT com status do sistema

![Display TFT](./Captura%20de%20tela%202025-05-22%20202255.png)

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
| ![Temp](./Captura%20de%20tela%202025-05-22%20202135.png) | ![Umid](./Captura%20de%20tela%202025-05-22%20202135.png) | ![Presença](./Captura%20de%20tela%202025-05-22%20202135.png) | ![Status](./Captura%20de%20tela%202025-05-22%20202135.png) |

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
🎮 Xbox | 🚗 Apaixonado por carros | 🎯 Foco: Casa top com área de festa com piscina 😎

---

