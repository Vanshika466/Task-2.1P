#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// Wi-Fi credentials
const char ssid[] = "Vanshika";          // your network SSID (name)
const char password[] = "123456788";    // your network password

// ThingSpeak settings
unsigned long myChannelNumber = 2624573;
const char * myWriteAPIKey = "H2YPVF4Z5QNNH4VX";

// DHT Sensor settings
#define DHTPIN 2        // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE);

// WiFi client object
WiFiClient client;

void setup() {
  Serial.begin(9600);
  dht.begin();
  
  // Connect to Wi-Fi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Read temperature and humidity from DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Celsius
  
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print values to Serial Monitor
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print("%  Temperature: ");
  Serial.print(t);
  Serial.println("°C");

  // Update ThingSpeak fields
  ThingSpeak.setField(1, t);  // Field 1 for temperature
  ThingSpeak.setField(2, h);  // Field 2 for humidity

  // Write to ThingSpeak
  int x = ThingSpeak.writeFields(2626205,H2YPVF4Z5QNNH4VX );
  
  if (x == 200) {
    Serial.println("Channel update successful.");
  } else {
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }

  // Wait 60 seconds before next update
  delay(60000);
}