                    
/* 
 *  Ex_21 - Adafruit IO-MQTT
 *  Control the LED from the Adafruit IO Dashboard
 *  Through MQTT protocol
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
 
// Libraries
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Replace with your network credentials
const char* ssid = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";

// Adafruit IO
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "percy_chen"
#define AIO_KEY         "aio_lkmlkdmflksmdlfksldkfmlskdmf"

// Functions
void connect();

// Create a WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[]     = AIO_SERVER;
// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[]   = AIO_KEY __DATE__ __TIME__;
const char MQTT_USERNAME[]   = AIO_USERNAME;
const char MQTT_PASSWORD[]   = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Setup a feed called 'gp12_LED' for subscribing to changes.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char LED_GP12[]  = AIO_USERNAME "/feeds/LED_GP12";
Adafruit_MQTT_Subscribe gp12_LED = Adafruit_MQTT_Subscribe(&mqtt, LED_GP12);
//Adafruit_MQTT_Publish a0_LDR = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/LDR_A0");


#define I2C1_SDA  18
#define I2C1_SCL  19
#define LED       12
#define IR_RECV   6
#define NEO_PIXEL 20
#define LED_COUNT 3

bool ledState = 0;

void setup() {
  
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  delay(10);
  Serial.print(F("Connecting to "));
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(F("."));
  }
  Serial.println();

  Serial.println(F("WiFi connected"));
  Serial.println(F("IP address: "));
  Serial.println(WiFi.localIP());
  
  // listen for events on the gp12_LED feed
  mqtt.subscribe(&gp12_LED);

  // connect to adafruit io
  connect();


}


// connect to adafruit io via MQTT
void connect() {

  Serial.print(F("Connecting to Adafruit IO... "));

  int8_t ret;

  while ((ret = mqtt.connect()) != 0) {

    switch (ret) {
      case 1: Serial.println(F("Wrong protocol")); break;
      case 2: Serial.println(F("ID rejected")); break;
      case 3: Serial.println(F("Server unavail")); break;
      case 4: Serial.println(F("Bad user/pass")); break;
      case 5: Serial.println(F("Not authed")); break;
      case 6: Serial.println(F("Failed to subscribe")); break;
      default: Serial.println(F("Connection failed")); break;
    }

    if(ret >= 0)
      mqtt.disconnect();

    Serial.println(F("Retrying connection..."));
    delay(5000);

  }

  Serial.println(F("Adafruit IO Connected!"));

}



void loop() {

  Adafruit_MQTT_Subscribe *subscription;

  
  // ping adafruit io a few times to make sure we remain connected
  if(! mqtt.ping(3)) {
    // reconnect to adafruit io
    if(! mqtt.connected())
      connect();
  }

  // this is our 'wait for incoming subscription packets' busy subloop
  while (subscription = mqtt.readSubscription(1000)) {

    // we only care about the gp12_LED events
    if (subscription == &gp12_LED) {

      // convert mqtt ascii payload to int
      char *value = (char *)gp12_LED.lastread;
      Serial.print(F("Received: "));
      Serial.println(value);

      // Apply message to gp12_LED
      String message = String(value);
      message.trim();
      if (message == "1") {ledState = HIGH;}
      if (message == "0") {ledState = LOW;}

      digitalWrite(LED, !ledState);
    }
  }

////Send an Update every 1000ms, Don't flood the server
//if (!a0_LDR.publish(analogRead(A0))) {Failed....} else { Pass...}

}                  
                    
                
