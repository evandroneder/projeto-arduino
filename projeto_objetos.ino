/*
 Projeto Objetos inteligentes interligados
 Nome:Evandro
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "EVANDRO4545";
const char* password = "neder98765";
const char* mqtt_server = "broker.mqtt-dashboard.com";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
String inString = "";
bool estadoSensor;
int Pinofalante = 0;
int frequencia = 0;
#define tempo 10

void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Conectando-se a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  randomSeed(micros());
  Serial.println("");
  Serial.println("WiFi Conectado!");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  inString="";
  Serial.print("Mensagem recebida: [");
  Serial.print(topic);
  Serial.print("] ");
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    inString+=(char)payload[i];
  }
  Serial.println();

}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar MQTT...");
    // Criando um ID de cliente aleatório
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Tentativa de conexão
    if (client.connect(clientId.c_str())) {
      Serial.println("Conectado!");
      // Depois de conectado, publica um anuncio...
      client.publish("objetos2/hello", "ESP Conectado");
      // ... e reassina/resubscribe
      client.subscribe("objetos2/trabalho");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando em 5 segundos");
      delay(5000);
    }
  }
}

void setup() {
  pinMode(BUILTIN_LED, OUTPUT);     // Inicializa o BUILTIN_LED pin como saída/output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(Pinofalante,OUTPUT); //Pino do buzzer
  pinMode(2, OUTPUT); //LED do ESP8266
  pinMode(4, OUTPUT); //LED Azul
  pinMode(5, OUTPUT); //LED Verde
  pinMode(10, INPUT); //PIR
  digitalWrite(4, HIGH);    
  delay(2000);                     
  digitalWrite(4, LOW);    
  delay(100);                      
  digitalWrite(5, HIGH);  
  delay(2000);                    
  digitalWrite(5, LOW);   
  delay(100);                   
  for (frequencia = 150; frequencia < 800; frequencia += 1) 
  {
    tone(Pinofalante, frequencia, tempo); 
    delay(1);
  }
  for (frequencia = 800; frequencia > 150; frequencia -= 1) 
  {
    tone(Pinofalante, frequencia, tempo); 
    delay(1);
  }
}

void loop() {
  estadoSensor = digitalRead(10);

  if (!client.connected()) {
    reconnect();
  }
    client.loop();
    alarme();
    delay(1000);
}
    void alarme(){

   if (estadoSensor == HIGH)  {
    
    long now = millis();
    if (now - lastMsg > 2000) {
      lastMsg = now;
      ++value;
      snprintf (msg, 75, "Movimentos: #%ld", value);
      Serial.print("Movimento detectado - ");
      Serial.println(msg);
      client.publish("objetos2/trabalho", msg);
      }

      
      delay(1);
      digitalWrite(4, HIGH); //Azul = Movimento
      digitalWrite(5, LOW);
      Serial.println("Movimento detectado!");
      
      for (frequencia = 150; frequencia < 1800; frequencia += 1) //150-1800
  {
        tone(Pinofalante, frequencia, tempo); 
        delay(1);
  }
      for (frequencia = 1800; frequencia > 150; frequencia -= 1) 
  {
        tone(Pinofalante, frequencia, tempo); 
        delay(1);
  }
  } 
  else 
  {
      digitalWrite(4, LOW);
      digitalWrite(5, HIGH); //Verde = Sem movimento
      Serial.println("Sem Movimento!");
  }

  }
