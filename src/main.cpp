#include <Arduino.h>

#include "SystemInit.hpp"
#include "WebView.hpp"
#include "EspNowHost.hpp"

WebView web(80);

void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) { 
  // Copies the sender mac address to a string
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
//   memcpy(&incomingReadings, incomingData, sizeof(incomingReadings));
  
//   board["id"] = incomingReadings.id;
//   board["temperature"] = incomingReadings.temp;
//   board["humidity"] = incomingReadings.hum;
//   board["readingId"] = String(incomingReadings.readingId);
//   String jsonString = JSON.stringify(board);
//   events.send(jsonString.c_str(), "new_readings", millis());
  
//   Serial.printf("Board ID %u: %u bytes\n", incomingReadings.id, len);
//   Serial.printf("t value: %4.2f \n", incomingReadings.temp);
//   Serial.printf("h value: %4.2f \n", incomingReadings.hum);
//   Serial.printf("readingID value: %d \n", incomingReadings.readingId);
//   Serial.println();
}


void setup()
{
    SystemInit sysInit;
    sysInit.init();

    // esp_now_register_recv_cb(OnDataRecv);

    // Init ESP-NOW
    initEspNow([](THMessage, String s){
        Serial.println(s);
    });

    web.load("/index.html");
    web.startServer();
}

JSONVar board;
int hum = 0;
void loop()
{
    static unsigned long lastEventTime = millis();
    static const unsigned long EVENT_INTERVAL_MS = 1000;
    if ((millis() - lastEventTime) > EVENT_INTERVAL_MS)
    {
        board["id"] = 1;
        board["temperature"] = 100;
        board["humidity"] = hum++;
        board["readingId"] = "Blabla";
        String jsonString = JSON.stringify(board);
        // web.sendEvent(jsonString.c_str(), "new_readings", millis());

        // web.sendEvent("ping",NULL,millis());
        lastEventTime = millis();
    }
}

/*********
  Rui Santos
  Complete project details at https://RandomNerdTutorials.com/esp-now-many-to-one-esp32/
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*********/

// #include <esp_now.h>
// #include <WiFi.h>

// // Structure example to receive data
// // Must match the sender structure
// typedef struct struct_message {
//   int id;
//   int x;
//   int y;
// }struct_message;

// // Create a struct_message called myData
// struct_message myData;

// // Create a structure to hold the readings from each board
// struct_message board1;
// struct_message board2;
// struct_message board3;

// // Create an array with all the structures
// struct_message boardsStruct[3] = {board1, board2, board3};

// // callback function that will be executed when data is received
// void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) {
//   char macStr[18];
//   Serial.print("Packet received from: ");
//   snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
//            mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
//   Serial.println(macStr);
//   memcpy(&myData, incomingData, sizeof(myData));
//   Serial.printf("Board ID %u: %u bytes\n", myData.id, len);
//   // Update the structures with the new incoming data
//   boardsStruct[myData.id-1].x = myData.x;
//   boardsStruct[myData.id-1].y = myData.y;
//   Serial.printf("x value: %d \n", boardsStruct[myData.id-1].x);
//   Serial.printf("y value: %d \n", boardsStruct[myData.id-1].y);
//   Serial.println();
// }
 
// void setup() {
//   //Initialize Serial Monitor
//   Serial.begin(115200);
  
//   //Set device as a Wi-Fi Station
//   WiFi.mode(WIFI_STA);

//   //Init ESP-NOW
//   if (esp_now_init() != ESP_OK) {
//     Serial.println("Error initializing ESP-NOW");
//     return;
//   }
  
//   // Once ESPNow is successfully Init, we will register for recv CB to
//   // get recv packer info
//   esp_now_register_recv_cb(OnDataRecv);
// }
 
// void loop() {
//   // Acess the variables for each board
//   /*int board1X = boardsStruct[0].x;
//   int board1Y = boardsStruct[0].y;
//   int board2X = boardsStruct[1].x;
//   int board2Y = boardsStruct[1].y;
//   int board3X = boardsStruct[2].x;
//   int board3Y = boardsStruct[2].y;*/

//   delay(10000);  
// }