#pragma once
#include <LittleFS.h>
#include <ArduinoJson.h>
#include <string.h>

extern const char csv_filepath[];
extern const char json_filepath[];

bool InitFileStorage();
bool FormatFileStorage();
bool StoreDataPoint(char* datapoint);
bool ClearBuffer();
bool PrintFile(const char* filepath);