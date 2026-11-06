# 🏭 Sistema de Supervisão e Automação para Planta de Telecom

![Status](https://img.shields.io/badge/Status-Conclu%C3%ADdo%20%2F%20Operacional-success)
![Hardware](https://img.shields.io/badge/Hardware-ESP32%20%7C%20ESP8266-blue)
![Linguagem](https://img.shields.io/badge/Linguagem-C%2B%2B-orange)
![Comunicação](https://img.shields.io/badge/Comunicação-WiFi%20%2F%20HTTP%20%2F%20Telegram-purple)

## 📌 Visão Geral
Este repositório reúne um conjunto de soluções de **IoT (Internet das Coisas)** e automação desenvolvidas para resolver problemas críticos de infraestrutura em uma planta de telecomunicações. 

Os sistemas foram projetados para monitorar redes AC/DC, bancos de baterias, temperatura e segurança patrimonial, enviando alertas em tempo real para os técnicos de plantão via Bot do Telegram.

## 💡 O Problema e a Solução
A planta enfrentava altos prejuízos com furtos de baterias e falta de monitoramento em tempo real de equipamentos críticos. As soluções comerciais de mercado eram caras e complexas.

**A Solução:** Desenvolvi módulos independentes utilizando microcontroladores ESP32 e ESP8266 (baixo custo) que leem sensores analógicos e digitais, processam a lógica de alarme e utilizam a API do Telegram como interface de notificação remota (sem custo de servidor ou SMS).

## ⚙️ Módulos do Projeto
O ecossistema é dividido nas seguintes pastas/soluções:

*   📁 **01-supervisao-gpon:** Monitoramento das centrais e status geral da planta.
*   📁 **02-alarme-baterias:** Sistema anti-furto que detecta a desconexão abrupta do banco de baterias e alerta os técnicos imediatamente.
*   📁 **03-controle-temperatura:** Leitura de sensores DHT22 para monitorar a climatização da sala de equipamentos e alertar sobre superaquecimento.
*   📁 **04-motor-alternancia-gmg:** Lógica de supervisão do Grupo Gerador (GMG) e alternância de carga.
*   📁 **05-logica-sazonalidade:** Rotinas programadas baseadas em horários e dias da semana.

## 🛠️ Tecnologias Utilizadas
*   **Hardware:** ESP32, ESP8266 (NodeMCU), Sensores de Tensão (Divisores), DHT22, Módulos Relés.
*   **Linguagem:** C++ (Arduino IDE).
*   **Bibliotecas:** `UniversalTelegramBot`, `WiFiManager`, `ArduinoJson`, `esp_task_wdt`.
*   **Comunicação:** Wi-Fi (HTTP) e API do Telegram.

## 🚀 Destaques Técnicos (Hard Skills)
*   **Watchdog Timer:** Implementação de `esp_task_wdt` para reiniciar o sistema em caso de travamento, garantindo alta disponibilidade.
*   **Histerese de Alarme:** Uso de flags para evitar o "flood" de mensagens (spam) no Telegram, enviando alertas apenas quando o estado do sensor muda.
*   **Divisor de Tensão:** Leitura precisa de tensões reais (AC e DC) a partir de pinos analógicos.
*   **Segurança (Whitelist):** Validação de ID de usuários no Telegram para garantir que apenas técnicos autorizados recebam os alertas.

## 📸 Galeria
*(Aqui você pode adicionar fotos do painel elétrico montado, dos ESP32 instalados ou do bot do Telegram recebendo as mensagens!)*

---
**Autor:** Desenvolvido por [Cristiano Cezarino Pereira](https://github.com/cristianocezarin)
