#pragma once

#include <WiFiManager.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "include/store.h"

void InitWifi();
void UpdateWifi();
void DownloadCSV();