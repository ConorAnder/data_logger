#pragma once

#include <WiFiManager.h>
#include <WebServer.h>
#include <LittleFS.h>

void InitWifi();
void UpdateWifi();
void DownloadCSV();