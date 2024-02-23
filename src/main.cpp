#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define RST_PIN -1 // Configurable, see typical pin layout above
#define SS_PIN D0 // Configurable, see typical pin layout above
#define LEDV D2
#define LEDR D1
#define buttonadd D5
#define buttondel D6
#define LEDJ D3
  
#define URLval "http://badges-api.glitch.me/validate/"
#define URLadd "http://badges-api.glitch.me/add/"
#define URLdel "http://badges-api.glitch.me/delete/"

int etatbuttonadd;
int etatbuttondel;
String URL;

MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance

void setup() {
  Serial.begin(115200); // Initialize serial communications with the PC
  while (!Serial); // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin(); // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));

  pinMode(LEDV, OUTPUT);
  pinMode(LEDR, OUTPUT);
  pinMode(LEDJ, OUTPUT);
  digitalWrite(LEDR, HIGH);
  pinMode(buttonadd, INPUT_PULLUP);
  pinMode(buttondel, INPUT_PULLUP);
  Serial.begin(115200);
  Serial.print("Connecting to WiFi");
  WiFi.begin("Name of the connection", "Password");

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");

}

  

void loop() {
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;

  }

  digitalWrite(LEDR, LOW);
  digitalWrite(LEDV, HIGH);
  delay(500);
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDV, LOW);

  String UID = "";

  for (byte i = 0; i< mfrc522.uid.size; i++) {
    if (mfrc522.uid.uidByte[i] < 0x10) {
      UID += "0";
    }
    UID += String(mfrc522.uid.uidByte[i], HEX);

  }

  UID.toUpperCase();
  etatbuttonadd = digitalRead(buttonadd);
  etatbuttondel = digitalRead(buttondel);

    

  if (etatbuttonadd == LOW) {
    URL = URLadd + UID;
    digitalWrite(LEDJ, HIGH);
    delay(500);
    digitalWrite(LEDJ, LOW);

  }

  else if (etatbuttondel == LOW){
    URL = URLdel + UID;
    digitalWrite(LEDJ, HIGH);
    delay(500);
    digitalWrite(LEDJ, LOW);

  }

  else {
    URL = URLval + UID;

  }

  HTTPClient http;
  http.begin(URL);
  int code = http.GET();
  delay(5000);

  if (code == HTTP_CODE_OK) {
    Serial.print("ok");

  }

  http.end();

}