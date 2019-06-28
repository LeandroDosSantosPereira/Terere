#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>

// Set these to run example.
#define FIREBASE_HOST "terereif.firebaseio.com"
#define FIREBASE_AUTH "ZOdOGqeISSSkKVTi9CzeOUqoYrjji68MHvX5LFyb"
#define WIFI_SSID "jhonatan nobre"
#define WIFI_PASSWORD "jhonatan nobre"
#define ID_GARRAFA "1"
//#define ID_USUARIO 
//#define UMIDADE "200"
//#define TEMPERATURA "1000"

//********************************
//Sensor
#define SS_PIN D4      //D2
#define RST_PIN D3     //D1

#include <SPI.h>
#include <MFRC522.h>
//UmiTemp
#include <DHT.h>

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
//********************************


//UmiTemp
#define DHTPIN D8
// Utilize a linha de acordo com o modelo do sensor
//#define DHTTYPE DHT11   // Sensor DHT11
#define DHTTYPE DHT22   // Sensor DHT 22  (AM2302)
//#define DHTTYPE DHT21   // Sensor DHT 21 (AM2301)

// Definicoes do sensor : pino, tipo
DHT dht(DHTPIN, DHTTYPE);

void setup() {


  //Led
  pinMode(D1,OUTPUT);

// Iniclaiza o sensor DHT
Serial.println("Aguardando dados...");  
  dht.begin();


  
  Serial.begin(9600);

//************************************
//Sensor
Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522

  //************************************

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("conectando");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("conectado: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
int envio=0;
void loop() {




  
//***********************************************
//Sensor
// Look for new cards
 
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
     
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
     
    return;
  }
  //Show UID on serial monitor
  Serial.println();
  Serial.print(" UID tag :");
  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
   String ID_USUARIO ; 

   
    
//***************************************************************
   
  if (content.substring(1) == "51 92 36 2E") //change UID of the card that you want to give access
  {
   String ID_USUARIO = content.substring(1);
    Serial.println(" Access Granted ");
    Serial.println(" Welcome Mr.Circuit ");
    digitalWrite(D1,HIGH);
    delay(1000);
    Serial.println(" Have FUN ");
    Serial.println();
    statuss = 1;

  
   //**********************************

    
//TempSensor
// Aguarda 2 segundos entre as medicoes
  delay(2000);
  
  // Leitura da umidade
  float UMIDADE = dht.readHumidity();
  // Leitura da temperatura (Celsius)
  float TEMPERATURA = dht.readTemperature();
  
  // Verifica se o sensor esta respondendo
  if (isnan(UMIDADE) || isnan(TEMPERATURA))
  {
    Serial.println("Falha ao ler dados do sensor DHT !!!");
    return;
  }

  // Mostra a temperatura no serial monitor e no display
  Serial.print("Temperatura: "); 
  Serial.print(TEMPERATURA);
  Serial.print(" *C  ");
  // Mostra a umidade no serial monitor e no display
  Serial.print("Umidade : "); 
  Serial.print(UMIDADE);
  Serial.println(" %");


    
    
//***************************************************************
//Firebase
  int sensorValue = analogRead(A0);   // Ler o pino Analógico A0 onde está o LDR
  float voltagem = sensorValue * (3.3 / 1024.0);   // Converter a leitura analógica (que vai de 0 - 1023) para uma voltagem (0 - 3.3V), quanto de acordo com a intensidade de luz no LDR a voltagem diminui.
  Serial.println(voltagem);   // Mostrar valor da voltagem no monitor serial
 
  if(envio!=1){
  
  Firebase.setString("id_usuario",  ID_USUARIO); 
  Firebase.setString("id_garrafa", ID_GARRAFA);
  Firebase.setFloat("umidade", UMIDADE);  
  Firebase.setFloat("temperatura", TEMPERATURA); 

  }
  delay(5000); // Espera 5 segundos
  
  digitalWrite(D1,LOW);
    
  }
  
  else   {
    Serial.println(" Access Denied ");
    delay(3000);
  }

  //***********************************************
  
  
}
