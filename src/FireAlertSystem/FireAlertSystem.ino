#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// ================= CONFIGURAÇÕES =====================
// WiFi
#define WIFI_SSID "Caue"
#define WIFI_PASSWORD "Caue2005"

// ThingSpeak
#define THINGSPEAK_API_KEY "H5JZ80FTUCS3113U"
#define CHANNEL_ID 2984134
#define UPDATE_INTERVAL 15000  // 15 segundos

// Sensores e atuadores
#define DHT_PIN 4             // DHT22 data pin
#define MQ2_PIN 34            // MQ-2 analog pin
#define FLAME_SENSOR_PIN 35   // (Opcional) Sensor de chama
#define LED_RED 25            // LED vermelho
#define LED_GREEN 26          // LED verde
#define BUZZER_PIN 27         // Buzzer

// Limiares
#define TEMP_THRESHOLD 50.0   // Temperatura em °C
#define GAS_THRESHOLD 400     // Gás em PPM
#define FLAME_THRESHOLD 100   // Chama (analógico)

#define DHTTYPE DHT22         // Tipo do sensor DHT
// =====================================================

DHT dht(DHT_PIN, DHTTYPE);

float temperature = 0;
int gasLevel = 0;
int flameLevel = 0;
bool isAlertActive = false;

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(MQ2_PIN, INPUT);
  pinMode(FLAME_SENSOR_PIN, INPUT);
  
  // Start DHT sensor
  dht.begin();
  
  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nConnected to WiFi");
  digitalWrite(LED_GREEN, HIGH);  // Turn on green LED when connected
}

void loop() {
  // Read sensors
  readSensors();
  
  // Check for fire conditions
  checkFireConditions();
  
  // Send data to ThingSpeak
  sendToThingSpeak();
  
  // Wait before next reading
  delay(UPDATE_INTERVAL);
}

void readSensors() {
  // Read temperature
  temperature = dht.readTemperature();
  if (isnan(temperature)) {
    Serial.println("Failed to read temperature!");
    temperature = 0;
  }
  
  // Read gas level
  gasLevel = analogRead(MQ2_PIN);
  gasLevel = map(gasLevel, 0, 4095, 0, 1000);  // Map to PPM
  
  // Read flame sensor
  flameLevel = analogRead(FLAME_SENSOR_PIN);
  
  // Print readings to Serial
  Serial.println("Sensor Readings:");
  Serial.println("Temperature: " + String(temperature) + "°C");
  Serial.println("Gas Level: " + String(gasLevel) + " PPM");
  Serial.println("Flame Level: " + String(flameLevel));
}

void checkFireConditions() {
  bool alertCondition = false;
  
  // Check each condition
  if (temperature > TEMP_THRESHOLD) {
    Serial.println("High temperature detected!");
    alertCondition = true;
  }
  
  if (gasLevel > GAS_THRESHOLD) {
    Serial.println("High gas level detected!");
    alertCondition = true;
  }
  
  if (flameLevel < FLAME_THRESHOLD) {  // Lower value means flame detected
    Serial.println("Flame detected!");
    alertCondition = true;
  }
  
  // Update alert status
  if (alertCondition != isAlertActive) {
    isAlertActive = alertCondition;
    updateAlertStatus();
  }
}

void updateAlertStatus() {
  if (isAlertActive) {
    digitalWrite(LED_RED, HIGH);
    digitalWrite(LED_GREEN, LOW);
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(LED_RED, LOW);
    digitalWrite(LED_GREEN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
  }
}

void sendToThingSpeak() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost!");
    return;
  }
  
  HTTPClient http;
  String url = "http://api.thingspeak.com/update?api_key=" + String(THINGSPEAK_API_KEY);
  
  // Add field data
  url += "&field1=" + String(temperature);
  url += "&field2=" + String(gasLevel);
  url += "&field3=" + String(flameLevel);
  url += "&field4=" + String(isAlertActive);
  
  http.begin(url);
  int httpResponseCode = http.GET();
  
  if (httpResponseCode > 0) {
    Serial.println("Data sent to ThingSpeak. Response code: " + String(httpResponseCode));
  } else {
    Serial.println("Error sending data to ThingSpeak");
  }
  
  http.end();
} 