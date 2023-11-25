
/* 
 *  P06 - Line Sensor Module Test
 *  Testing TomatoCube 5 Chennel IR
 *  Line Sensor Module
 *  
 *
 *  I2C
 *  --  GP18 (SDA) 
 *  --  GP19 (SCL) 
 * 
 */
 

#define IR_RECEIVE_PIN      6
#define IR_RECV 6

#include <Adafruit_NeoPixel.h>
#include <Wire.h>

#define OLED_RESET -1
#define I2C_ADDRESS 0x3C

#define LINESENI2CADDR 0x17

#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"

#define ONBOARD_LED 12
#define ONBOARD_SW 7

#define SPK_OUT   22 

#define NEOPIXEL  20 
#define NUMPIXELS 3 

#define ENDSTP_SW_A 27
#define ENDSTP_SW_B 28
#define LDR_PIN A0  // 26 - ADC0

#define SERVO_A 16
#define SERVO_B 17

#define MotorA_1 8
#define MotorA_2 9
#define MotorB_1 10
#define MotorB_2 11


#define I2C1_SDA 18
#define I2C1_SCL 19

#define UART0_TX 0
#define UART0_RX 1


Adafruit_NeoPixel pixels(NUMPIXELS, NEOPIXEL, NEO_GRB + NEO_KHZ800);
//Adafruit_SSD1306 display(SCRN_WIDTH, SCRN_HEIGHT, &Wire1, OLED_RESET, 400000L, 400000L);
SSD1306AsciiWire oled(Wire1);


unsigned long lastSample = 0;

uint8_t lineSensorData;

//Read a byte from the LineSensor
uint8_t lineSensorRead(int address) {
    uint8_t _data;
    Wire1.requestFrom(address, 1);
    if(Wire1.available()) {
        _data = Wire1.read();
    }
    return _data;
}


void setup() {
    // USB UART
    Serial.begin(115200);

    pinMode(UART0_TX, INPUT_PULLUP);
    pinMode(UART0_RX, INPUT_PULLUP);

    
    Wire1.setSDA(I2C1_SDA);
    Wire1.setSCL(I2C1_SCL);
    Wire1.begin(); 

    oled.begin( &Adafruit128x32, I2C_ADDRESS);
    oled.setFont(System5x7);
    oled.clear();
    oled.print("Hello Line Sensor!");

    pinMode(IR_RECV, INPUT_PULLUP);

    pinMode(ENDSTP_SW_A, INPUT_PULLUP);
    pinMode(ENDSTP_SW_B, INPUT_PULLUP);
    pinMode(ONBOARD_SW, INPUT_PULLUP);
    pinMode(ONBOARD_LED, OUTPUT);


    pinMode(MotorA_1, OUTPUT);
    pinMode(MotorA_2, OUTPUT);
    pinMode(MotorB_1, OUTPUT);
    pinMode(MotorB_2, OUTPUT);
    digitalWrite(MotorA_1, LOW);
    digitalWrite(MotorA_2, LOW);
    digitalWrite(MotorB_1, LOW);
    digitalWrite(MotorB_2, LOW);
    
    pinMode(SPK_OUT, OUTPUT);
    noTone(SPK_OUT);
    delay(500);



}

void loop() {
    
    if (millis() > lastSample + 1000) {
        lastSample = millis();
        lineSensorData = lineSensorRead(LINESENI2CADDR) & 0x1F; // 5 LSN only
        oled.clear();
        oled.println("LineSensor");
        for (uint8_t aBit = 0; aBit < 5 ; aBit++)
          oled.print(bitRead(lineSensorData, aBit) ? '1' : '0');
 
    }


}
                    
                
