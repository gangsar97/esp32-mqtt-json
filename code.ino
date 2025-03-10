#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <DHT.h>

// WIFI SETUP
#define WIFI_SSID "ssid"
#define WIFI_PASS "password"

// MQTT SETUP
#define MQTT_BROKER "broker" 
#define MQTT_PORT 1883
#define MQTT_USERNAME "user"
#define MQTT_PASSWORD "pasword"
#define MQTT_TOPIC "topic/subtopic"

// DHT PIN
#define DHTPIN 4
#define DHTTYPE DHT11

WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);

void connectWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    Serial.print("Connecting to WiFi...");
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nWiFi connected!");
}

void connectMQTT() {
    while (!client.connected()) {
        Serial.print("Connecting to MQTT...");
        if (client.connect("ESP32_Client", MQTT_USERNAME, MQTT_PASSWORD)) {
            Serial.println("connected!");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" retrying in 5 seconds...");
            delay(5000);
        }
    }
}

void sendMQTTData() {
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        return;
    }

    // Create JSON object
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["temperature"] = temperature;
    jsonDoc["humidity"] = humidity;

    char buffer[256];
    serializeJson(jsonDoc, buffer);

    client.publish(MQTT_TOPIC, buffer);
    Serial.print("Published: ");
    Serial.println(buffer);
}

void setup() {
    Serial.begin(115200);
    dht.begin();

    connectWiFi();
    client.setServer(MQTT_BROKER, MQTT_PORT);

    connectMQTT();
}

void loop() {
    if (!client.connected()) {
        connectMQTT();
    }
    client.loop();

    sendMQTTData();
    delay(5000); // Send data every 5 seconds
}
