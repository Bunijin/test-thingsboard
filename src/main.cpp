#include <DHTesp.h>
#include <WiFi.h>
#include <ThingsBoard.h>
#include <Arduino_MQTT_Client.h>

#define pinDht 15
DHTesp dhtSensor;

#define WIFI_SSID "xxxxx"  // Changed from WIFI_AP to WIFI_SSID
#define WIFI_PASS "----------"

#define TB_SERVER "thingsboard.cloud"
#define TOKEN "IHySAkLlhsHLTZcSgG0C"

constexpr uint16_t MAX_MESSAGE_SIZE = 128U;

WiFiClient espClient;
Arduino_MQTT_Client mqttClient(espClient);
ThingsBoard tb(mqttClient, MAX_MESSAGE_SIZE);

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  int attempts = 0;

  WiFi.disconnect(true);  // Disconnect from any previous connections
  WiFi.mode(WIFI_STA);    // Set WiFi mode to station
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED && attempts < 20) {
    delay(500);
    Serial.print(".");
    attempts++;
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\nFailed to connect to WiFi.");
    Serial.print("WiFi status: ");
    Serial.println(WiFi.status());
  } else {
    Serial.println("\nConnected to WiFi");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void connectToThingsBoard() {
  if (!tb.connected()) {
    Serial.println("Connecting to ThingsBoard server");

    if (!tb.connect(TB_SERVER, TOKEN)) {
      Serial.println("Failed to connect to ThingsBoard");
    } else {
      Serial.println("Connected to ThingsBoard");
    }
  }
}

void sendDataToThingsBoard(float temp, int hum) {
  String jsonData = "{\"temperature\":" + String(temp) + ", \"humidity\":" + String(hum) + "}";
  tb.sendTelemetryJson(jsonData.c_str());
  Serial.println("Data sent");
}

void setup() {
  Serial.begin(115200);
  dhtSensor.setup(pinDht, DHTesp::DHT22);
  connectToWiFi();
  connectToThingsBoard();
}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    connectToWiFi();
  }

  TempAndHumidity data = dhtSensor.getTempAndHumidity();
  float temp = data.temperature;
  int hum = data.humidity;

  Serial.println(temp);
  Serial.println(hum);

  if (!tb.connected()) {
    connectToThingsBoard();
  }

  sendDataToThingsBoard(temp, hum);

  delay(3000);

  tb.loop();
}
