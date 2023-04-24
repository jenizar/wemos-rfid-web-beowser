/**
Useful links
https://wiki.wemos.cc/products:d1:d1_mini
https://cdn-images-1.medium.com/max/1400/1*YKc8KpAfMrlhrOLmNjdRwQ.png (D1 full pinout)
https://github.com/Jorgen-VikingGod/ESP8266-MFRC522
https://github.com/miguelbalboa/rfid
d1 mini rc52 wiring
https://discourse-cdn-sjc1.com/business5/uploads/mydevices/original/2X/e/ecedba79dc05f2c0b02b7fba8b3da2681590a11a.jpg
RST  - D3
MISO - D6
MOSI - D7
SCK  - D5
SDA  - D8
*/

// https://forum.arduino.cc/t/convert-mfrc522-uid-to-string-byte-array-to-string/613945

#include "ESP8266WiFi.h"
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <MFRC522.h>

constexpr uint8_t RST_PIN =  0;          // Configurable, see typical pin layout above 18
constexpr uint8_t SS_PIN =  15;         // Configurable, see typical pin layout above  16

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

const char* ssid = "al-husna_EXT2"; //wifi ssid / wifi name
const char* password = "sekolahdasar"; //wifi password

String serverName = "http://192.168.0.101/arduino/rfid/postdata.php?tagid=";

void setup() {
  Serial.begin(115200);   // Initialize serial communications with the PC
  //delay(1000);
  delay(250);
  Serial.println("Setup");
  
  while (!Serial);    // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4)
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
  Serial.println("Setup done");

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");  
}

void loop() {

    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){
      WiFiClient client;
      HTTPClient http;
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    delay(50);
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    delay(50);
    return;
  }

  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  char str[32] = "";
   array_to_string(mfrc522.uid.uidByte, 4, str); //Insert (byte array, length, char array for output)
  Serial.println();
  Serial.println(str); //Print the output uid string  Serial.println();

  String serverPath = serverName + String (str);

      // Your Domain name with URL path or IP address with path
      http.begin(client, serverPath.c_str());
      
      // Send HTTP GET request
      int httpResponseCode = http.GET();
      
      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        String payload = http.getString();
        Serial.println(payload);
        Serial.println(serverPath);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      // Free resources
      http.end();
    }
    else {
      Serial.println("WiFi Disconnected");
    }
  
  // Dump debug info about the card; PICC_HaltA() is automatically called
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

}

void array_to_string(byte array[], unsigned int len, char buffer[])
{
   for (unsigned int i = 0; i < len; i++)
   {
      byte nib1 = (array[i] >> 4) & 0x0F;
      byte nib2 = (array[i] >> 0) & 0x0F;
      buffer[i*2+0] = nib1  < 0xA ? '0' + nib1  : 'A' + nib1  - 0xA;
      buffer[i*2+1] = nib2  < 0xA ? '0' + nib2  : 'A' + nib2  - 0xA;
   }
   buffer[len*2] = '\0';
}

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
} 
