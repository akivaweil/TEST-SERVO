#include <ESP32Servo.h>
#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include <WebSocketsServer.h>

// Constants
//const int SERVO_PIN = 47; // Make this non-const to allow changing
const int MIN_ANGLE = 0;
const int MAX_ANGLE = 180;
const unsigned long SERIAL_BAUD = 115200;

// WiFi Credentials
const char* ssid = "Everwood";
const char* password = "Everwood-Staff";

// Static IP Configuration
IPAddress local_IP(192, 168, 1, 244);
IPAddress gateway(192, 168, 1, 1);   // Usually 192.168.1.1
IPAddress subnet(255, 255, 255, 0); // Standard for /24 networks

// Web Server and WebSocket
AsyncWebServer server(80);             // Create AsyncWebServer object on port 80
WebSocketsServer webSocket = WebSocketsServer(81); // Create WebSocket server on port 81

// HTML Page (served on HTTP port 80)
// Using raw string literal R"rawliteral(...)rawliteral" for easy HTML embedding
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>ESP32 Servo Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; text-align: center; padding: 20px; }
    h1 { color: #333; }
    .control-section { margin-bottom: 30px; padding: 15px; border: 1px solid #ccc; border-radius: 8px; background-color: #f9f9f9; }
    label { display: block; margin-bottom: 10px; font-weight: bold; }
    input[type=range] { width: 80%; max-width: 400px; margin-bottom: 10px; }
    input[type=number] { width: 80px; padding: 8px; margin-bottom: 10px; }
    button { padding: 10px 20px; font-size: 1em; cursor: pointer; border-radius: 5px; border: none; background-color: #007bff; color: white; }
    button:hover { background-color: #0056b3; }
    #angleValue { font-size: 1.2em; font-weight: bold; color: #007bff; }
    #status { margin-top: 20px; font-style: italic; color: #555; }
  </style>
</head>
<body>
  <h1>ESP32 Servo Control</h1>
  
  <div class="control-section">
    <label for="servoAngle">Servo Angle: <span id="angleValue">90</span>&deg;</label>
    <input type="range" id="servoAngle" min="0" max="180" value="90" oninput="updateAngleValue(this.value)" onchange="sendAngle(this.value)">
  </div>

  <div class="control-section">
    <label for="servoPin">Change Servo Pin</label>
    <input type="number" id="servoPinInput" placeholder="Pin #">
    <button onclick="sendPinChange()">Set Pin</button>
  </div>

  <div id="status">Connecting to WebSocket...</div>

<script>
  var gateway = `ws://${window.location.hostname}:81/`;
  var websocket;
  var currentPin = 47; // Keep track of the current pin

  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    updateStatus("Connecting...");
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage;
    websocket.onerror   = onError; 
  }

  function onOpen(event) {
    console.log('Connection opened');
    updateStatus("Connected. Initial pin: " + currentPin);
    // You could request initial state here if needed
  }

  function onClose(event) {
    console.log('Connection closed');
    updateStatus("Disconnected. Retrying in 2s...");
    setTimeout(initWebSocket, 2000);
  }

  function onMessage(event) {
    console.log('Received: ', event.data);
    updateStatus("Received: " + event.data);
    // Update current pin if message confirms pin change
    if (event.data.startsWith("Servo pin changed to")) {
        try {
            currentPin = parseInt(event.data.split(" ").pop().replace('.', ''));
            document.getElementById('servoPinInput').placeholder = "Current: " + currentPin;
        } catch (e) { console.error("Error parsing pin from message:", e); }
    }
  }

 function onError(event) {
    console.error("WebSocket Error: ", event);
    updateStatus("WebSocket Error! See console.");
 }

  function sendAngle(angle) {
    console.log("Sending angle: " + angle);
    if(websocket.readyState === WebSocket.OPEN) {
      websocket.send(angle);
    } else {
      console.log("WebSocket not open.");
      updateStatus("Error: WebSocket not connected.");
    }
  }
  
  function sendPinChange() {
    var pinValue = document.getElementById('servoPinInput').value;
    if (pinValue === null || pinValue === '') {
        updateStatus("Please enter a pin number.");
        return;
    }
    console.log("Sending pin change: pin:" + pinValue);
     if(websocket.readyState === WebSocket.OPEN) {
      websocket.send("pin:" + pinValue);
      document.getElementById('servoPinInput').value = ''; // Clear input after sending
    } else {
      console.log("WebSocket not open.");
      updateStatus("Error: WebSocket not connected.");
    }
  }

  function updateAngleValue(value) {
    document.getElementById('angleValue').innerText = value;
  }
  
  function updateStatus(message) {
      document.getElementById('status').innerText = message;
  }

  window.addEventListener('load', initWebSocket);
</script>
</body>
</html>
)rawliteral";

// --- Global Variables ---
int currentServoPin = 47; // Initial servo pin, now changeable
Servo myServo;

// Function declarations
void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length);

void setup() {
  // Initialize serial communication
  Serial.begin(SERIAL_BAUD);
  while (!Serial) {
    ; // Wait for serial port to connect
  }
  
  // Initialize servo
  myServo.attach(currentServoPin);
  myServo.write(MIN_ANGLE);  // Set initial position
  Serial.printf("Servo initialized on pin %d.\n", currentServoPin);
  
  // Print instructions (update pin info)
  Serial.println("Servo Control Program");
  Serial.println("-------------------");
  Serial.print("Enter angle between ");
  Serial.print(MIN_ANGLE);
  Serial.print(" and ");
  Serial.print(MAX_ANGLE);
  Serial.println(" degrees");

  // Connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start WebSocket server and attach event handler
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
  Serial.println("WebSocket server started on port 81.");

  // Start Web Server - Serve the HTML page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });
  server.begin();
  Serial.println("HTTP server started on port 80. Access via http://192.168.1.244");

  Serial.println("Setup complete. Waiting for connections...");
}

void loop() {
  // Handle WebSocket client connections and messages
  webSocket.loop();

  // Minimal delay to prevent busy-waiting
  delay(10);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  switch (type) {
    case WStype_DISCONNECTED:
      Serial.printf("[%u] Disconnected!\n", num);
      break;

    case WStype_CONNECTED: {
      IPAddress ip = webSocket.remoteIP(num);
      Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);
      // Send welcome message including current pin
      String welcomeMsg = "Connected to ESP32 Servo Control! Current pin: " + String(currentServoPin);
      webSocket.sendTXT(num, welcomeMsg);
      break;
    }

    case WStype_TEXT: {
      Serial.printf("[%u] Received Text: %s\n", num, payload); // Log original payload

      String message = String((char*)payload);
      message.trim();

      // Check if it's a pin change command
      if (message.startsWith("pin:")) {
        String pinStr = message.substring(4); // Get the part after "pin:"
        pinStr.trim();
        char* pinEndPtr;
        long newPinLong = strtol(pinStr.c_str(), &pinEndPtr, 10);

        // Validate pin number conversion
        if (*pinEndPtr == '\0' && pinStr.length() > 0) {
          int newPin = (int)newPinLong;
          // Basic validation (e.g., ensure it's within typical GPIO range)
          // More specific validation could be added based on ESP32-S3 datasheet
          if (newPin >= 0 && newPin <= 48) { // Example range check
            if (myServo.attached()) {
              myServo.detach();
              Serial.printf("Detached servo from pin %d\n", currentServoPin);
            }
            currentServoPin = newPin;
            myServo.attach(currentServoPin);
            // Optionally set to a default angle after changing pin
            myServo.write(MIN_ANGLE); 
            Serial.printf("Servo pin changed to %d and attached.\n", currentServoPin);
            String response = "Servo pin changed to " + String(currentServoPin) + ".";
            webSocket.sendTXT(num, response);
          } else {
            Serial.printf("Error: Invalid pin number %d\n", newPin);
            webSocket.sendTXT(num, "Error: Invalid pin number provided.");
          }
        } else {
          Serial.printf("Error: Invalid pin command format: %s\n", payload);
          webSocket.sendTXT(num, "Error: Invalid pin command format. Use 'pin:<number>'.");
        }
      } 
      // Otherwise, assume it's an angle command
      else {
        char* angleEndPtr;
        long angle_long = strtol(message.c_str(), &angleEndPtr, 10);

        if (*angleEndPtr == '\0' && message.length() > 0) {
          int angle = (int)angle_long;
          if (angle >= MIN_ANGLE && angle <= MAX_ANGLE) {
            myServo.write(angle);
            Serial.printf("Moving servo (pin %d) to %d degrees\n", currentServoPin, angle);
            String response = "Servo (pin " + String(currentServoPin) + ") moved to " + String(angle) + " degrees.";
            webSocket.sendTXT(num, response);
          } else {
            Serial.printf("Error: Angle %d out of range (%d-%d)\n", angle, MIN_ANGLE, MAX_ANGLE);
            String errorMsg = "Error: Angle must be between " + String(MIN_ANGLE) + " and " + String(MAX_ANGLE) + ".";
            webSocket.sendTXT(num, errorMsg);
          }
        } else {
          Serial.printf("Error: Invalid command format received: %s\n", payload);
          webSocket.sendTXT(num, "Error: Invalid command. Send angle (0-180) or 'pin:<number>'.");
        }
      }
      break;
    }

    case WStype_BIN:
      Serial.printf("[%u] get binary length: %u\n", num, length);
      // Handle binary data if needed
      break;

    case WStype_ERROR:
    case WStype_FRAGMENT_TEXT_START:
    case WStype_FRAGMENT_BIN_START:
    case WStype_FRAGMENT:
    case WStype_FRAGMENT_FIN:
    case WStype_PING: // Optional: Implement PONG response if needed
    case WStype_PONG: // Optional: Handle PONG response if you sent a PING
      break;
  }
}
