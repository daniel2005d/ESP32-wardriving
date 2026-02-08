# ESP32 WiFi Geo-Logger

Proyecto basado en ESP32 que permite detectar redes WiFi, georreferenciarlas usando GNSS (GPS6MV2 / NEO-6M) y almacenarlas en una microSD, mostrando información relevante en una pantalla TFT ST7789.

El sistema está pensado como un nodo autónomo de recolección de datos, útil para análisis de cobertura WiFi, estudios de campo, wardriving educativo, IoT y experimentación con GNSS.

## 🚀 Características

📡 Escaneo de redes WiFi (SSID, BSSID, RSSI, canal, cifrado)

🛰️ Obtención de coordenadas GPS (latitud y longitud)

🗺️ Asociación de cada red WiFi con su ubicación geográfica

💾 Almacenamiento de datos en archivo sobre microSD

🖥️ Visualización en tiempo real mediante pantalla ST7789

⚡ Funcionamiento autónomo sobre ESP32

## Hardware

* ESP32 (cualquier variante con WiFi)

* GPS6MV2 / NEO-6M con antena activa

* Pantalla TFT ST7789 (SPI)

* Módulo microSD

* Antena GPS

* Fuente de alimentación estable

## PINOUT

## GPS

|  ESP32    |  GPS6MV2      |
|-----------|---------------|
|   3V3     |   VCC         |
|   GPIO17  |   RX          |
|   GPIO16  |   TX          |
|   GND     |   GND         |


## MICROSD

| ESP32     |    MICROSD    |
|-----------|---------------|
|   GND     |      GND      |
|   5V      |      VCC      |
|   GPIO19  |     MISO      |
|   GPIO23  |     MOSI      |
|   GPIO18  |     SCK       |
|   GPIO5   |     CS        |


## LED GPS

| ESP32     |    LED     | Resistencia (4.7k Ohm)
|-----------|------------|-------------
|  GPIO13   |            |     X
|           |  (+) LED   |     X
|  GND      |  (-) LED   |

## LED WIFI

| ESP32     |    LED     | Resistencia (4.7k Ohm)
|-----------|------------|-------------
|  GPIO12   |            |     X
|           |  (+) LED   |     X
|  GND      |  (-) LED   |


Verde: WiFi
Amarillo: GPS
ROJO: Error => Pendiente.