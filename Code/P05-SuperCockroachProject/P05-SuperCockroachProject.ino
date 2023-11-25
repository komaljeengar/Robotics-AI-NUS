                    
/*
 *  P05 - SuperCockroachProject
 *
 *  Raspberry Pico-W Running WebSocket Server Interfacing to the
 *  LED on the TomatoCube Pico MainBoard
 *  
 *  LEDs
 *  --  GP20
 *  
 *  IR_RECV
 *  --  GP6
 *  
 *  NeoPixel
 *  -- GP20
 *
 *  I2C
 *  --  GP18 (SDA) 
 *  --  GP19 (SCL) 
 * 
 */

#include <WiFi.h>
#include <WiFiMulti.h>
#include <WiFiClientSecure.h>

#include <WebSocketsServer.h>
#include <WebServer.h>

WiFiMulti WiFiMulti;
WebSocketsServer webSocket = WebSocketsServer(81);
WebServer server(80);


#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include <Adafruit_NeoPixel.h>

// I2C address
#define SR04_I2CADDR 0x57
#define OLED_I2CAADR 0x3C

SSD1306AsciiWire oled(Wire1);

#define I2C1_SDA  18
#define I2C1_SCL  19
#define LED       12
#define IR_RECV   6
#define NEO_PIXEL 20
#define LED_COUNT 3

// Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

bool ledState = 0;
unsigned long lastUltraSonicUpdate = 0;

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>Pico-W Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
  html {
    font-family: Arial, Helvetica, sans-serif;
    text-align: center;
  }
  </style>
<title>ESP Web Server</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="icon" href="data:,">
</head>
<body>
  <div class="topnav">
    <h1>TomatoCube - CockroachBot WebSocket Server</h1>
  </div>
  <div class="content">
    <div class="card">
      <h2>LED - GP12</h2>
      <p class="state">state: <span id="state">-</span></p>
      <p><button id="button" class="button">Toggle</button></p>
    </div>
  </div>
<script>
  var gateway = `ws://${window.location.hostname}:81`;
  var websocket;
  window.addEventListener('load', onLoad);
  function initWebSocket() {
    console.log('Trying to open a WebSocket connection...');
    websocket = new WebSocket(gateway);
    websocket.onopen    = onOpen;
    websocket.onclose   = onClose;
    websocket.onmessage = onMessage; // <-- add this line
  }
  function onOpen(event) {
    console.log('Connection opened');
  }
  function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
  }
  function onMessage(event) {
    var state;
    if (event.data == "LED=1"){
      state = "ON";
    }
    else if (event.data == "LED=0"){
      state = "OFF";
    }
    document.getElementById('state').innerHTML = state;
  }
  function onLoad(event) {
    initWebSocket();
    initButton();
  }
  function initButton() {
    document.getElementById('button').addEventListener('click', toggle);
  }
  function toggle(){
    websocket.send('toggle');
  }
</script>
</body>
</html>
)rawliteral";


// TODO: Add in Motor Control Function

// TODO: Add in UltraSonic Sensor Reading Function
                    
                    
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void webSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {

    switch(type) {
        case WStype_DISCONNECTED:
            Serial.printf("[%u] Disconnected!\n", num);
            break;
        case WStype_CONNECTED:
            {
                IPAddress ip = webSocket.remoteIP(num);
                Serial.printf("[%u] Connected from %d.%d.%d.%d url: %s\n", num, ip[0], ip[1], ip[2], ip[3], payload);

                // send message to client
                webSocket.sendTXT(num, "Connected");
                webSocket.sendTXT(num, ledState? "LED=1": "LED=0");
            }
            break;
        case WStype_TEXT:
            Serial.printf("[%u] get Text: %s\n", num, payload);
            if (strcmp((char*)payload, "toggle") == 0) {
              ledState = !ledState;
              webSocket.broadcastTXT(ledState? "LED=1": "LED=0");
            } 
            // TODO: Some Cal to the Motor Driving Function here...
            else if (strcmp((char*)payload, "Forward") == 0) {
              Serial.printf("Forward\n");
            } else if (strcmp((char*)payload, "Backward") == 0) {
              Serial.printf("Backward\n");
            } else if (strcmp((char*)payload, "Left") == 0) {
              Serial.printf("Left\n");
            } else if (strcmp((char*)payload, "Right") == 0) {
              Serial.printf("Right\n");
            } else if (strcmp((char*)payload, "Stop") == 0) {
              Serial.printf("Stop\n");
            }

            break;
        case WStype_BIN:
        case WStype_ERROR:      
        case WStype_FRAGMENT_TEXT_START:
        case WStype_FRAGMENT_BIN_START:
        case WStype_FRAGMENT:
        case WStype_FRAGMENT_FIN:
          break;
    }

}



void setup(){
  Wire1.setSDA(I2C1_SDA);
  Wire1.setSCL(I2C1_SCL);
  Wire1.begin(); 
  oled.begin(&Adafruit128x32, OLED_I2CAADR); 
  
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
                                        
  // TODO: Add in Motor Pin Initializing functions
                                      
  delay(2000);

  Serial.begin(115200);

  oled.setFont(Adafruit5x7);
  oled.clear();
  oled.println("WebSocket Server");
  
  WiFiMulti.addAP(ssid, password);
  
  while(WiFiMulti.run() != WL_CONNECTED) {
      delay(100);
  }
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());
  oled.println(WiFi.localIP());
  
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);

  // handle index
  server.on("/", []() {
      // send index.html
      server.send(200, "text/html", index_html);
  });
  
  server.onNotFound(handleNotFound);

  server.begin();                                      
}

void loop() {
  webSocket.loop();
  server.handleClient();
  digitalWrite(LED, ledState);
  if (millis() >= lastUltraSonicUpdate + 1000) {
    char ultraBuffer[40];
    int dist = 0; //TODO: Looks like it needs a sensor's value
    sprintf(ultraBuffer, "Ultrasonic=%d", dist);
    webSocket.broadcastTXT(ultraBuffer);
    lastUltraSonicUpdate = millis();
    delay(2);//allow the cpu to switch to other tasks
  }
  else
    delay(2);//allow the cpu to switch to other tasks
}
                    
                
