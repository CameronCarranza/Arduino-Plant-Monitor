#include <ESP8266WiFi.h>

// Network Configuration
const char* ssid     = "";
const char* password = "";
const char* api_key  = "random_api_key";
const char* host     = "192.168.1.3";
const int host_port  = 3000;

// Amount of Multiplexer Inputs being currently used (plug in devices starting at y0).
int amount_of_mux_inputs = 2;

// Digital Select Pins for Multiplexer to use
int s0 = 12; 
int s1 = 13;
int s2 = 15;

// Analog Pin to connect to Multiplexer
uint8_t AnalogSensor = A0;

/**
 * Initial Setup
 */
void setup(){  
  Serial.begin(115200);
  
  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);

  connectToWifi();
}

 
/**
 * Required Loop
 */
void loop () {
  sendSensorDataToHost();
  delay(600000); // TODO: Replace with Sleep state
}

/**
 * Send the data from attached sensors to the host as JSON.
 */
void sendSensorDataToHost() {

  // Used to loop through the Multiplexer and select the y0-7 pins.
  int count = 0; 

  // Value of Select Pins on the Multiplexer
  int r0 = 0; //value of select pin at the 4051 (s0)
  int r1 = 0; //value of select pin at the 4051 (s1)
  int r2 = 0; //value of select pin at the 4051 (s2)
  
  Serial.print("Connecting To ");
  Serial.println(host);
  
  WiFiClient client;
  if ( !client.connect(host, host_port) ) {
    Serial.println("Connection Failed.");
    return;
  }

  // Create JSON Data
  String JsonData = "{\"sensorData\":{";

  // Read Inputs through Multiplexer and add them to the JSON payload.
  for (count=0; count <= (amount_of_mux_inputs - 1); count++) {

    int sensorNumber = count + 1;

    // select the bit  
    r0 = bitRead(count,0);    // use this with arduino 0013 (and newer versions)     
    r1 = bitRead(count,1);    // use this with arduino 0013 (and newer versions)     
    r2 = bitRead(count,2);    // use this with arduino 0013 (and newer versions)     

    digitalWrite(s0, r0);
    digitalWrite(s1, r1);
    digitalWrite(s2, r2);
    
    JsonData = JsonData + "\"sensor"+sensorNumber+"\":"+analogRead(AnalogSensor);    

    // Add Comma if there are more Items to add.
    if ( count != (amount_of_mux_inputs - 1) ) {
      JsonData = JsonData + ",";
    }
    
  } 
  JsonData = JsonData + "}}";  

  Serial.println(JsonData);

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
