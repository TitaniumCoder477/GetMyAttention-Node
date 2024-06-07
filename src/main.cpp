#include <Arduino.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include "wifi_profile.h"
#include "Behavior.h"
#include "FlashCharlieWingBank.h"
#include "BlinkHuzzahLED.h"

AsyncWebServer server(80);
//const char* PARAM_MESSAGE;
std::vector<std::unique_ptr<Behavior>> behaviors;

void startWebServer();

void notFound(AsyncWebServerRequest *request) {
    request->send(404, "text/plain", "Not found");
}

void setup() {
    // Connect to wifi
    Serial.begin(115200);
    Serial.println();
    WiFi.begin(network_name, network_pass);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    Serial.print("Connected, IP address: ");
    Serial.println(WiFi.localIP());
    // Provision the behaviors
    behaviors.push_back(std::unique_ptr<Behavior> { new FlashCharlieWingBank() });
    behaviors.push_back(std::unique_ptr<Behavior> { new BlinkHuzzahLED() });
    // Start the web server
    startWebServer();
}

void loop() {
    for(size_t i = 0; i < behaviors.size(); i++) {
        behaviors[i].get()->loop();
    }
}

void startWebServer() {
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        Serial.println("INFO: Client submitted GET request...");
        request->send(200, "text/plain", "Welcome to the GetMyAttention-Node!");
    });
    server.on("/on", HTTP_POST, [] (AsyncWebServerRequest *request) {
        Serial.println("INFO: Client submitted POST ON request...");
        for(size_t i = 0; i < behaviors.size(); i++) {
            behaviors[i].get()->setOn();
        }
        request->send(200, "text/plain", "Node is on.");
    });
    // Send a GET request to <IP>/get?message=<message>
    server.on("/off", HTTP_POST, [] (AsyncWebServerRequest *request) {
        Serial.println("INFO: Client submitted POST OFF request...");
        for(size_t i = 0; i < behaviors.size(); i++) {
            behaviors[i].get()->setOff();
        }
        /* String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        } */
        request->send(200, "text/plain", "Node is off.");
    });
    // Send a GET request to <IP>/get?message=<message>
    server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
        Serial.println("INFO: Client submitted GET STATE request...");
        bool allOn = true;
        bool allOff = true;
        for(size_t i = 0; i < behaviors.size(); i++) {
            if(!behaviors[i].get()->isOn())
                allOn = false;
            if(behaviors[i].get()->isOn())
                allOff = false;
        }
        /* String message;
        if (request->hasParam(PARAM_MESSAGE)) {
            message = request->getParam(PARAM_MESSAGE)->value();
        } else {
            message = "No message sent";
        } */
        if(allOn)
            request->send(200, "text/plain", "Node is on.");
        else if(allOff)
            request->send(200, "text/plain", "Node is off.");
        else
            request->send(200, "text/plain", "Some of the node's behaviors are on and some are off.");
    });
    // Send a GET request to <IP>/get?message=<message>
    server.on("/reboot", HTTP_POST, [] (AsyncWebServerRequest *request) {
        Serial.println("INFO: Client submitted POST REBOOT request...");
        wdt_disable();
        wdt_enable(WDTO_15MS);
        while(1) {}
    });
    server.onNotFound(notFound);
    server.begin();
}