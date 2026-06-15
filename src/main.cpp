#include <Arduino.h>
#include "Lpf2/Local/Serial.hpp"
#include "device.h"

Lpf2::Local::Parser parser1;
Lpf2::Local::Parser parser2;

Esp32s3Uart uart1(1);
Esp32s3Uart uart2(2);

void setup() {
    Serial.begin(115200);
    lpf2_set_runtime_log_level(LPF2_LOG_LEVEL_DEBUG);

    if (uart1.begin(115200, SERIAL_8N1, 1, 2)) { // tx pin 1, rx pin 2
        uart1.setUartPinsState(false); // Set pins to active state
        Serial.println("UART1 initialized successfully.");
    } else {
        Serial.println("Failed to initialize UART1.");
    }

    if (uart2.begin(115200, SERIAL_8N1, 41, 42)) { // tx pin 41, rx pin 42
        uart2.setUartPinsState(false); // Set pins to active state
        Serial.println("UART2 initialized successfully.");
    } else {
        Serial.println("Failed to initialize UART2.");
    }

    parser1.init(&uart1);
    parser2.init(&uart2);
}

void loop() {
    
    if (Serial.available()) {
        uint8_t c = Serial.read();
        if (c == 0x03) {
            // Ctrl+C received
            ESP.restart();
        }
    }

    auto messages1 = parser1.update();
    for (const auto &msg : messages1) {
        Serial.print("TX: ");
        Lpf2::Local::Parser::printMessage(msg);
    }

    // if (uart1.available()) {
    //     uint8_t c = uart1.read();
    //     Serial.print("UART1 Received: 0x");
    //     Serial.println(c, HEX);
    // }

    auto messages2 = parser2.update();
    for (const auto &msg : messages2) {
        Serial.print("RX: ");
        Lpf2::Local::Parser::printMessage(msg);
    }

    vTaskDelay(1);
}