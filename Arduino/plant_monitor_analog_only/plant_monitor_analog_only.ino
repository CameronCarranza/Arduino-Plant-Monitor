#include <ESP8266WiFi.h>

// Network Configuration
const char* ssid     = "";
const char* password = "";
const char* api_key  = "";
const char* host     = "192.168.1.2";
const int host_port  = 3000;

// Sensor Pin Configuration
uint8_t Sensors[] = {A0};

/**
 * Initial Setup
 */
void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);

  connectToWifi();
}

/**
 * Required Loop
 */
void loop() {
  sendSensorDataToHost();
  delay(600000); // TODO: Replace with Sleep state
}

/**
 * Send the data from attached sensors to the host as JSON.
 */
void sendSensorDataToHost() {

  int i;
  
  Serial.print("Connecting To ");
  Serial.println(host);
  
  WiFiClient client;
  if ( !client.connect(host, host_port) ) {
    Serial.println("Connection Failed.");
    return;
  }

  // Create JSON Data
  String JsonData = "{\"sensorData\":{";
  for (i=0; i < sizeof(Sensors); i = i + 1) {
    
    int sensorNumber = i + 1;
    
    JsonData = JsonData + "\"sensor"+sensorNumber+"\":"+analogRead(Sensors[i]);

    // Add Comma if there are more Items to add.
    if ( sensorNumber != sizeof(Sensors) ) {
      JsonData = JsonData + ",";
    }
    
  }
  JsonData = JsonData + "}}";  

  // Send JSON to API
  client.println("POST /devices/" + String(api_key) + " HTTP/1.1");
  client.println("Host: " + String(host));
  client.println("User Agent: Arduino/1.0");
  client.println("Cache-Control: no-cache");
  client.println("Connnection: close");
  client.println("Content-Type: application/json");
  client.print("Content-Length: ");
  client.println(JsonData.length());
  client.println();
  client.println(JsonData);

  Serial.println("Sensor Data submitted to API.");
}

/**
 * Connect to the Host.
 */
void connectToWifi() {
  delay(100);

  Serial.println();
  Serial.println();
  Serial.println("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println("IP Address: ");
  Serial.println(WiFi.localIP());
}
