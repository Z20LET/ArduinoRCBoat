#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
extern const char index_html[] PROGMEM;

// Replace with your network credentials
const char* ssid = "Arduino Boat";
const char* password = "JamaraVenezia";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Pins where motors are connected
const int motorPin1 = 18;
const int motorPin2 = 19;
bool connectionLost = false;

// Function to handle slider1 value
void handleSlider1(AsyncWebServerRequest *request) {
  String inputMessage;
  if (request->hasParam("value")) {
    inputMessage = request->getParam("value")->value();
    int motorValue1 = inputMessage.toInt();
    analogWrite(motorPin1, motorValue1);
  }
  request->send(200, "text/plain", "OK");
}

// Function to handle slider2 value
void handleSlider2(AsyncWebServerRequest *request) {
  String inputMessage;
  if (request->hasParam("value")) {
    inputMessage = request->getParam("value")->value();
    int motorValue2 = inputMessage.toInt();
    analogWrite(motorPin2, motorValue2);
  }
  request->send(200, "text/plain", "OK");
}

// Function to handle signal strength requests
void handleSignalStrength(AsyncWebServerRequest *request) {
  int32_t rssi = WiFi.RSSI();
  request->send(200, "text/plain", String(rssi));
}

void setup() {
  // Serial port for debugging purposes
  Serial.begin(115200);

  // Setting up motor pins
  pinMode(motorPin1, OUTPUT);
  pinMode(motorPin2, OUTPUT);

  // Connecting to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println(WiFi.localIP());

  // Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Handle slider value changes
  server.on("/motor1", HTTP_GET, handleSlider1);
  server.on("/motor2", HTTP_GET, handleSlider2);

  // Handle signal strength requests
  server.on("/signal-strength", HTTP_GET, handleSignalStrength);

  // Start server
  server.begin();
}

void loop() {
  // Check Wi-Fi connection status
  if (WiFi.status() != WL_CONNECTED) {
    if (!connectionLost) {
      Serial.println("WiFi connection lost! Stopping motors.");
      // Shut down the motors
      analogWrite(motorPin1, 0);
      analogWrite(motorPin2, 0);
      connectionLost = true;
    }
  } else {
    connectionLost = false;
  }
}
