#pragma once

#include <LittleFS.h>

bool InitFileStorage();
bool FormatFileStorage();
bool StoreDataPoint(char* datapoint);
bool PrintCSV();