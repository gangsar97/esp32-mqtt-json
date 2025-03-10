# esp32-mqtt-json

Basic example to send data in JSON format over MQTT.

External Library Requirement for Arduino IDE:
- PubSubClient https://github.com/knolleary/pubsubclient
- ArduinoJson https://github.com/bblanchon/ArduinoJson
- DHT Sensor Library https://github.com/adafruit/DHT-sensor-library

# Limitation

According to PubSubClient library, this code only can publish data in QoS 0, and subscribe in QoS 0 and 1 
