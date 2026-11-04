#pragma once

#include <WiFiManager.h>
#include <WebServer.h>
#include <LittleFS.h>
#include "include/store.h"
#include <Preferences.h>

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
#define TOKEN_LENGTH 64
extern char url[];
extern char api[];
extern char email[];
extern char pass[];

void InitWifi();
void UpdateWifi();
void DownloadCSV();
void ConfigServerParams();
void UpdateSetParams();