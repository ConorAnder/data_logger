#pragma once

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#define DB_FILEPATH "/esp32_001/readings"   // Possible chain of multiple devices?

#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "include/wifi.h"
#include "include/store.h"

void InitDatabase();
bool SendData();
bool ShouldSendData();
void processData(AsyncResult &aResult);
void UpdateApp();