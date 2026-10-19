#include "include/store.h"

const char filepath[] = "/datapoints.csv";

bool InitFileStorage() {
    if (!LittleFS.begin(false)) {
        Serial.println("LittleFS initialization failed");
        return true;
    }

    return false;
}

bool FormatFileStorage() {
    LittleFS.begin(true);

    if (!LittleFS.format()) 
        return true;

    LittleFS.end();
    return InitFileStorage();
}

bool StoreDataPoint(char* datapoint) {
    File f = LittleFS.open(filepath, FILE_APPEND);
    if (!f) return true;

    bool status = false;
    if (!f.println(datapoint)) status = true;

    f.close();
    return status;
}

bool PrintCSV() {
    File f = LittleFS.open("/datapoints.csv", FILE_READ);
    if (!f) return true;

    while (f.available())
        Serial.write(f.read());

    f.close();
    return false;
}