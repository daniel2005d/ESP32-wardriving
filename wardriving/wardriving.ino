/*
* +-----------+---------------+
* |  ESP32    |  GPS6MV2      |
* +===========+===============+
* |   3V3     |   VCC         |
* +-----------+---------------+
* |   GPIO17  |   RX          |
* +-----------+---------------+
* |   GPIO16  |   TX          |
* +-----------+---------------+
* |   GND     |   GND         |
* +-----------+---------------+

* +-----------+---------------+
* | ESP32     +    MICROSD    +
* +===========+===============+
* |   GND     |      GND      |
* +-----------+---------------+
* |   5V      |      VCC      |
* +-----------+---------------+
* |   GPIO19  |     MISO      |
* +-----------+---------------+
* |   GPIO23  |     MOSI      |
* +-----------+---------------+
* |   GPIO18  |     SCK       |
* +-----------+---------------+
* |   GPIO5   |     CS        |
* +-----------+---------------+
*/

#include <TinyGPS++.h>
//#include <EEPROM.h>
#include <SPI.h>
#include <SD.h>
#include <WiFi.h>


//#define EEPROM_SIZE 1
#define RX 16
#define TX 17

#define MAX_NETWORKS 20
#define WIFI_LED 12
#define GPS_LED 13

int sck = 18;
int miso = 19;
int mosi = 23;
int cs = 5;


struct Location{
  double latitude;
  double longitude;
};

struct Networks{
  String ssid;
  long signal;
  uint8_t bssid[6];
  byte encryption;
};

TinyGPSPlus gps;
HardwareSerial GPS(2);

File sdCard;
Networks networks[MAX_NETWORKS];
int networkCount = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(WIFI_LED, OUTPUT);
  pinMode(GPS_LED, OUTPUT);
  banner();
  memset(networks, 0, sizeof(networks));
  GPS.begin(9600, SERIAL_8N1, RX, TX); // RX, TX
  
  
  SPI.begin(sck, miso, mosi, cs);
  if (!SD.begin(cs)){
      error("SDCard initialization failed!");
  }
  else{
    Serial.println("SDCard Ready.");
    //printSavedLocations();
  }
    checkSD();
  //initScreen();
  WiFi.begin();
}


void checkSD(){
  Serial.println("Checking SDCard");
  uint8_t cardType = SD.cardType();
  if (cardType == CARD_NONE) {
    error("No se detecta tarjeta");
    return;
  }

  if (cardType == CARD_MMC) Serial.println("MMC");
  else if (cardType == CARD_SD) Serial.println("SDSC");
  else if (cardType == CARD_SDHC) Serial.println("SDHC");
  else Serial.println("DESCONOCIDA");


   uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("Tamaño: %llu MB\n", cardSize);

  // Prueba de escritura
  File file = SD.open("/test.txt", FILE_WRITE);
  if (!file) {
    error("❌ Error al abrir archivo para escribir");
    return;
  }

  file.println("ESP32 SD test OK");
  file.close();
  Serial.println("✅ Escritura OK");
}

void saveToFile(Location &location){
  sdCard = SD.open("/coordinates.txt", FILE_APPEND);  
  for (int i = 0; i < networkCount; i++) {
    String line = buildLine(location, networks[i]);
    Serial.println(line);
    sdCard.println(line);
  }

  sdCard.close();
  Serial.println("Saved positions");
}

void loop() {
  digitalWrite(WIFI_LED, LOW);
  digitalWrite(GPS_LED, LOW);
  
  Location loc;
  if (scanGPS(loc)){
      digitalWrite(GPS_LED, HIGH);
      if (scanWiFi()){
        digitalWrite(WIFI_LED, HIGH);
        Serial.println("Redes encontradas:");
        saveToFile(loc);
      }
  }

  delay(5000);
 
}

bool scanWiFi(){
  Serial.println("WIFI");
  memset(networks, 0, sizeof(networks));
  networkCount = 0;
  int total_networks = WiFi.scanNetworks();
  if (total_networks <= 0) {
    return false;
  }

  Serial.print("Total networks: ");
  Serial.println(total_networks);

  networkCount = min(total_networks, MAX_NETWORKS);

  for(int i=0;i<networkCount;i++){
    
    networks[i].ssid = WiFi.SSID(i);
    networks[i].encryption = WiFi.encryptionType(i);
    networks[i].signal = WiFi.RSSI(i);
    memcpy(networks[i].bssid, WiFi.BSSID(i), 6);
  }

  return true;
}

bool scanGPS(Location &location){
  
    while (GPS.available()) {
          char c = GPS.read();
          //Serial.print(c);
          gps.encode(c);
      }
      
      if (gps.location.isValid() && gps.location.isUpdated()){
          double lat = gps.location.lat();
          double lon = gps.location.lng();
          if (lat > 0){
              location.latitude = lat;
              location.longitude = lon;
              return true;
          }
          
      }

      return false;
}

String buildLine(Location &loc, Networks &net) {
  String line = "";

  // BSSID
    String bssid = String(net.bssid[5], HEX);
    bssid += ":";
    bssid += String(net.bssid[4], HEX);
    bssid += ":";
    bssid += String(net.bssid[3], HEX);
    bssid += ":";
    bssid += String(net.bssid[2], HEX);
    bssid += ":";
    bssid += String(net.bssid[1], HEX);
    bssid += ":";
    bssid += String(net.bssid[0], HEX);
    
  
  //for (int i = 0; i < networkCount; i++) {
    line += net.ssid;
    line += "|";
    line += bssid;
    line += "|";
    line += String(net.signal);
    line +="|";
    line += String(net.encryption);
    line +="|";
    line += String(loc.latitude);
    line +="|";
    line += String(loc.longitude);
  //}

  return line;
}

void error(String message){
    Serial.println(message);
    
      int status = HIGH;
      for (int i=0;i<=20;i++){
          
          if (i%2!=0){
            status = LOW;
          }
          else{
            status = HIGH;
          }

          digitalWrite(GPS_LED, status);
          digitalWrite(WIFI_LED, status);
          delay(100);
        }
}

void banner(){
  int status = HIGH;    
  // Serial.println("_,---._  ,-- Beijing Mushroom");
  // Serial.println(",' _____ `.");
  // Serial.println("'-( \ / )-'");
  // Serial.println("\(_)/");
  // Serial.println(") (");
  // Serial.println("\"\"\"");

    for (int i=0;i<=3;i++){
      
      if (i%2!=0){
        status = LOW;
      }
      else{
        status = HIGH;
      }

      digitalWrite(GPS_LED, status);
      delay(200);
      digitalWrite(WIFI_LED, status);
      delay(200);
    }
}
