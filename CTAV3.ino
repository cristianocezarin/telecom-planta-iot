

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////  CODER CRISTIANO CEZARINO  /////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*
 * 
 * 
Equivalencia das saidas Digitais entre NodeMCU e ESP8266 (na IDE do Arduino)
NodeMCU - ESP8266


A0 = 20;
D0 = 16;
D1 = 5;
D2 = 4; 
D3 = 0;
D4 = 2;
D5 = 14;
D6 = 12;
D7 = 13;
D8 = 15;
D9 = 3;
D10 = 1;
*/

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <DNSServer.h>            
#include <ESP8266WebServer.h>     
#include <WiFiManager.h>         
#include <ArduinoJson.h>

#define BOT_TOKEN_LENGTH 50

#define PinoSensor                 20  // A0 LEITURA TENSÃO DC CONSUMIDOR
#define pino13                     13  // D7 RELAY FALTA DE FASE S
#define pino12                     12  // D6 RELAY FALTA DE FASE T
#define pino14                     14  // D5 DHT 22 SENSOR TEMPERATURA E UNIDADE
#define pino16                     16  // D0
#define pino5                      5   // D1
#define pino4                      4   // D2
#define pino2                      2   // D4
#define pino15                     15  // D8
#define pino0                      0   // D3 RELAY FALTA DE FASE R
#define pino3                      3   // D9 RX THERMOSTAT TEMPERATURA
#define pino10                     1   // D10 
#define DHTTYPE DHT22
#define INTERVAL 1000
#define SENDER_ID_COUNT 2

char botToken[BOT_TOKEN_LENGTH] = "5956584624:AAGjUYzcLW4KSLdmZP_hTwrpFqsTxheMK2Q";

WiFiClientSecure client;
UniversalTelegramBot bot(botToken, client);
uint32_t lastCheckTime = 0;
DHT dht(pino14, DHTTYPE);

String validSenderIds[SENDER_ID_COUNT] = {"853254203","1410086182"};
String defaultChatId = "853254203"; 


int Bot_mtbs = 200;  
long Bot_lasttime;   
bool Start = false;

int pinoSensor = 0;
int sensorStatus = 0;
int sensorStatus2 = 0;
int sensorStatus3 = 0;
int sensorStatus4 = 0;
char stado;
int flag = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;

int temp_max = 29;
int Vconsumidor_min = 46;

unsigned long previousMillis = 0;
const long interval = 60000;
unsigned long previousMillis2 = 0;
const long intervalo = 60000;

float tensaoEntrada = 0.0; //VARIAVEL PARA ARMAZENAR O VALOR DE TENSÃO DE ENTRADA DO SENSOR
float tensaoMedida = 0.0; //VARIAVEL PARA ARMAZENAR O VALOR DA TENSÃO MEDIDA PELO SENSOR

float valorR1 = 295000.0; //VALOR DO RESISTOR 1 DO DIVISOR DE TENSÃO
float valorR2 = 30000.0; // VALOR DO RESISTOR 2 DO DIVISOR DE TENSÃO
int leituraSensor = 0; //VARIÁVEL PARA ARMAZENAR A LEITURA DO PINO ANALÓGICO

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////// void handleNewMessages(int numNewMessages)
    
     void handleNewMessages(int numNewMessages)
     {
     for (int i=0; i<numNewMessages; i++) 
     {
     String defaultChatId = String(bot.messages[i].chat_id);  
     String senderId = String(bot.messages[i].from_id);
     Serial.println("senderId: " + senderId); 
     boolean validSender = validateSender(senderId);
     if(!validSender) 
     {
     bot.sendMessage(defaultChatId, "<b>\xF0\x9F\x9A\xAB Usuário sem permissão de acesso!</b>", "HTML"); 
     continue; 
     }
     String text = bot.messages[i].text;   
  

      if (text == "/mvCA1") {
      if (sensorStatus) {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase R \xE2\x9D\x95</b>", "HTML");
      } else {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase R \xF0\x9F\x86\x97</b>", "HTML");
      }
      } 

       if (text == "/mvCA2") {
      if (sensorStatus2) {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase S \xE2\x9D\x95</b>", "HTML");
      } else {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase S \xF0\x9F\x86\x97</b>", "HTML");
      }
      }
      
      if (text == "/mvCA3") {
      if (sensorStatus3) {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase T \xE2\x9D\x95</b>", "HTML");
      } else {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase T \xF0\x9F\x86\x97</b>", "HTML");
      }
      }
      
      if( text == "/mvCONS") {
      String message = "*\xE2\x9A\xA1TensÃ£o Consumidores* " + String(tensaoMedida, 1) + " *Vcc\xE2\x9D\x95*\n";         
      bot.sendMessage(defaultChatId, message, "Markdown");
      }
      
      if (text == "/mCLIMA") {
      if (sensorStatus4) {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xF0\x9F\x8C\xA1 Temperatura Externa, acima de 30Â°C\xE2\x9D\x95</b>", "HTML");
      } else {
        bot.sendMessage(defaultChatId, "<b>\xF0\x9F\x8C\xA1 Temperatura Externa, abaixo de 30Â°C\xE2\x9D\x95</b>", "HTML");
      }
      }
      
      if( text == "/mTEMP") {
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature();     
      if (isnan(humidity) || isnan(temperature)) {
      bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 Falha na leitura no DHT sensor!</b>", "HTML");
      return;
      
      } else  {
        
      String message = "*\xF0\x9F\x8C\xA1 Temperatura Local* " + String(temperature, 2) + " *Â°C\xE2\x9D\x95*\n";
      message += "*\xE2\x99\xA8 Umidade do Ar* " + String(humidity, 2)+ " *%\xE2\x9D\x95*\n\n";      
      bot.sendMessage(defaultChatId, message, "Markdown");
      }
      }
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

       if (text == "/GetStatus") {

       if (sensorStatus) {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase R \xE2\x9D\x95</b>", "HTML");
      } else {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase R \xF0\x9F\x86\x97</b>", "HTML");    
      }
      
       if (sensorStatus2) {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase S \xE2\x9D\x95</b>", "HTML");
      } else {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase S \xF0\x9F\x86\x97</b>", "HTML");
      }
      
      if (sensorStatus3) {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase T \xE2\x9D\x95</b>", "HTML");
      } else {
        bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase T \xF0\x9F\x86\x97</b>", "HTML");
      }
 
      if (PinoSensor) {
      String message = "*\xE2\x9A\xA1Tensao Consumidores* " + String(tensaoMedida, 1) + " *Vcc\xE2\x9D\x95*\n";         
      bot.sendMessage(defaultChatId, message, "Markdown");

      }

      if (sensorStatus4) {
       bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xF0\x9F\x8C\xA1 Temperatura Externa, acima de 30Â°C\xE2\x9D\x95</b>", "HTML");
      } else {
       bot.sendMessage(defaultChatId, "<b>\xF0\x9F\x8C\xA1 Temperatura Externa, abaixo de 30Â°C\xE2\x9D\x95</b>", "HTML");
      }
 
      if (pino14) {
      float humidity = dht.readHumidity();
      float temperature = dht.readTemperature(); 
      if (isnan(humidity) || isnan(temperature)) {
      bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 Falha na leitura no DHT sensor!</b>", "HTML");
      return;
         
      } else  {
           
      String message = "*\xF0\x9F\x8C\xA1 Temperatura Local* " + String(temperature, 2) + " *Â°C\xE2\x9D\x95*\n";
      message += "*\xE2\x99\xA8 Umidade do Ar* " + String(humidity, 2)+ " *%\xE2\x9D\x95*\n\n";      
      bot.sendMessage(defaultChatId, message, "Markdown");
     
      }
      }
      }
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   
            
      if (text == "/start") {
      String welcome = "*\xE2\x84\xB9 Seja bem vindo ao Centro de Alarmes IE CTAV\xE2\x9D\x95*\n";
      welcome = welcome + "\n";
      welcome = welcome + "_Click nos comandos para visualizar os eventos_\xF0\x9F\x91\x87\n";
      welcome = welcome + "\n";
      welcome = welcome + "\xF0\x9F\x94\x84/GetStatus : _Informa o estado geral_\n";
      welcome = welcome + "\xE2\x9A\xA1/mvCA1  : _Tensão da rede Fase R_\n";
      welcome = welcome + "\xE2\x9A\xA1/mvCA2  : _Tensão da rede Fase S_\n";
      welcome = welcome + "\xE2\x9A\xA1/mvCA3  : _Tensão da rede Fase T_\n";
      welcome = welcome + "\xE2\x9A\xA1/mvCONS : _Informa Tensão dos Consumidores CC_\n"; 
      welcome = welcome + "\xF0\x9F\x8C\xA1/mCLIMA : _Informa Temperatura Externa_\n"; 
      welcome = welcome + "\xF0\x9F\x8C\xA1/mTEMP  : _Temperatura e Umidade local_\n";
      bot.sendMessage(defaultChatId, welcome, "Markdown");

      bot.sendMessage(defaultChatId, "<b>Aguardando comando\xE2\x8F\xB3</b>", "HTML");

     }

     }

     }
     
     boolean validateSender(String senderId)
     {
    
     for(int i=0; i<SENDER_ID_COUNT; i++)
     {
   
     if(senderId == validSenderIds[i])
     {
      return true;
      }
      }
  
  
      return false;
      }

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

     void setup() {
  
     Serial.begin(9600);

     pinMode(13, INPUT_PULLUP);
 
     pinMode(12, INPUT_PULLUP);

     pinMode(3, INPUT_PULLUP);
  
     pinMode(0, INPUT_PULLUP);

     pinMode(20, INPUT); 
  
     pinMode(14, OUTPUT);
     digitalWrite(14, LOW);

     pinMode(4, OUTPUT);
     digitalWrite(4, LOW);

     pinMode(5, OUTPUT);
     digitalWrite(5, LOW);
  
     pinMode(15, OUTPUT);
     digitalWrite(15, LOW);

     pinMode(16, OUTPUT);
     digitalWrite(16, LOW);

     pinMode(2, OUTPUT);
     digitalWrite(2, LOW);
  
  
    dht.begin();
    delay(10);
    Serial.println("read bot token");
    Serial.println(botToken);

    WiFiManager wifiManager;
    WiFiManagerParameter custom_bot_id("botid", "Bot Token", botToken, 50);
    wifiManager.addParameter(&custom_bot_id);
    wifiManager.autoConnect("CCP_Automation");
    strcpy(botToken, custom_bot_id.getValue());
    
    client.setInsecure();
    
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);
   
    }
    
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////    

    void loop() 

    {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); 
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval)
    {
    previousMillis = currentMillis;

    if (temp_max <= temperature)
    {
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); 
    String message = "*\xE2\x9A\xA0 Temperatura Alta* " + String(temperature, 2) + " *Â°C\xE2\x9D\x95*\n";
    message += "*\xE2\x99\xA8 Umidade do Ar* " + String(humidity, 2)+ " *%\xE2\x9D\x95*\n\n";      
    bot.sendMessage(defaultChatId, message, "Markdown"); 
    bot.sendMessage(cristiano, message, "Markdown");  
    }
    }

    if (millis() > Bot_lasttime + Bot_mtbs)  {
    int numNewMessages = bot.getUpdates(bot.last_message_received + 1);

    while (numNewMessages) {
      Serial.println("got response");
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
    }

    if (Serial.available() > 0) {
    stado = Serial.read();
    Serial.println(stado);

    }

    if (stado == 'R') { 
    Serial.println("Reset");
    WiFi.disconnect();
    Serial.println("Dq");
    delay(500);
    ESP.reset();
    delay(5000);
  }

  ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

 if ( (digitalRead(pino0) == LOW) && (flag == 0) ) {

    bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase R \xE2\x9D\x95</b>", "HTML");
    bot.sendMessage(cristiano, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase R \xE2\x9D\x95</b>", "HTML");
    sensorStatus = 1;
    flag = 1;
    delay(500);
  }

  if ((digitalRead(pino0) == HIGH)&& (flag == 1) ) {
     sensorStatus = 0;
     bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase R \xF0\x9F\x86\x97</b>", "HTML");
     bot.sendMessage(cristiano, "<b>\xE2\x9A\xA1 Rede CA Fase R \xF0\x9F\x86\x97</b>", "HTML");
     flag = 0;
     delay(500);
  }

    if ( (digitalRead(pino13) == LOW) && (flag2 == 0) ) {

    bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase S \xE2\x9D\x95</b>", "HTML");
    bot.sendMessage(cristiano, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase S \xE2\x9D\x95</b>", "HTML");
    sensorStatus2 = 1;
    flag2 = 1;
    delay(500);
  }

  if ((digitalRead(pino13) == HIGH)&& (flag2 == 1) ) {
     sensorStatus2 = 0;
     bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase S \xF0\x9F\x86\x97</b>", "HTML");
     bot.sendMessage(cristiano, "<b>\xE2\x9A\xA1 Rede CA Fase S \xF0\x9F\x86\x97</b>", "HTML");
     flag2 = 0;
     delay(500);
  }

  if ( (digitalRead(pino12) == LOW) && (flag3 == 0) ) {

    bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase T \xE2\x9D\x95</b>", "HTML");
    bot.sendMessage(cristiano, "<b>\xE2\x9A\xA0 \xE2\x9A\xA1 Falha rede CA Fase T \xE2\x9D\x95</b>", "HTML");
    sensorStatus3 = 1;
    flag3 = 1;
    delay(500);
  }

  if ((digitalRead(pino12) == HIGH)&& (flag3 == 1) ) {
     sensorStatus3 = 0;
     bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA1 Rede CA Fase T \xF0\x9F\x86\x97</b>", "HTML");
     bot.sendMessage(cristiano, "<b>\xE2\x9A\xA1 Rede CA Fase T \xF0\x9F\x86\x97</b>", "HTML");
     flag3 = 0;
     delay(500);
  }

   if ( (digitalRead(pino3) == LOW) && (flag4 == 0) ) {

    bot.sendMessage(defaultChatId, "<b>\xE2\x9A\xA0 \xF0\x9F\x8C\xA1 Temperatura Externa, acima de 30Â°C\xE2\x9D\x95</b>", "HTML");
    bot.sendMessage(cristiano, "<b>\xE2\x9A\xA0 \xF0\x9F\x8C\xA1 Temperatura Externa, acima de 30Â°C\xE2\x9D\x95</b>", "HTML");
    sensorStatus4 = 1;
    flag4 = 1;
    delay(500);
  }

  if ((digitalRead(pino3) == HIGH)&& (flag4 == 1) ) {
     sensorStatus4 = 0;
     bot.sendMessage(defaultChatId, "<b>\xF0\x9F\x8C\xA1 Temperatura Externa, abaixo de 30Â°C\xE2\x9D\x95</b>", "HTML");
     bot.sendMessage(cristiano, "<b>\xF0\x9F\x8C\xA1 Temperatura Externa, abaixo de 30Â°C\xE2\x9D\x95</b>", "HTML");
     flag4 = 0;
     delay(500);
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    leituraSensor = analogRead(pinoSensor);                      //FAZ A LEITURA DO PINO ANALÓGICO E ARMAZENA NA VARIÁVEL O VALOR LIDO
    tensaoEntrada = (leituraSensor * 5.0) / 1024.0;              //VARIÁVEL RECEBE O RESULTADO DO CÁLCULO
    tensaoMedida = tensaoEntrada / (valorR2/(valorR1+valorR2));  //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR
    unsigned long currentMillis2 = millis();
    if (currentMillis2 - previousMillis2 >= intervalo)
    {
    previousMillis2 = currentMillis2;
    
    if (Vconsumidor_min >= tensaoMedida)
    {
    leituraSensor = analogRead(pinoSensor);                       //FAZ A LEITURA DO PINO ANALÓGICO E ARMAZENA NA VARIÁVEL O VALOR LIDO
    tensaoEntrada = (leituraSensor * 5.0) / 1024.0;               //VARIÁVEL RECEBE O RESULTADO DO CÁLCULO
    tensaoMedida = tensaoEntrada / (valorR2/(valorR1+valorR2));   //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR
    String message = "*\xE2\x9A\xA0 Tensão baixa Consumidor* " + String(tensaoMedida, 1) + " *Vcc\xE2\x9D\x95*\n";         
    bot.sendMessage(cristiano, message, "Markdown");
    bot.sendMessage(defaultChatId, message, "Markdown");       
    }
    }
    leituraSensor = analogRead(pinoSensor);                       //FAZ A LEITURA DO PINO ANALÓGICO E ARMAZENA NA VARIÁVEL O VALOR LIDO
    tensaoEntrada = (leituraSensor * 5.0) / 1024.0;               //VARIÁVEL RECEBE O RESULTADO DO CÁLCULO
    tensaoMedida = tensaoEntrada / (valorR2/(valorR1+valorR2));   //VARIÁVEL RECEBE O VALOR DE TENSÃO DC MEDIDA PELO SENSOR
   
    Serial.print("Tensão DC Consumidores: ");                     //IMPRIME O TEXTO NA SERIAL
    Serial.print(tensaoMedida,2);                                 //IMPRIME NA SERIAL O VALOR DE TENSÃO DC MEDIDA E LIMITA O VALOR A 2 CASAS DECIMAIS
    Serial.println("V");                                          //IMPRIME O TEXTO NA SERIAL
    delay(200);                                                   //INTERVALO DE 500 MILISSEGUNDOS

   } 
   

