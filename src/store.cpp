#include "include/store.h"

const char csv_filepath[] = "/datapoints.csv";
const char json_filepath[] = "/datapoints.jsonl";

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
    // Store CSV backup
    File cf = LittleFS.open(csv_filepath, FILE_APPEND);
    if (!cf) return true;

    bool status = false;
    if (!cf.println(datapoint)) status = true;

    cf.close();

    // Create JSON buffer datapoint
    const char* delimiter = ",";
    char* str = strtok(datapoint, delimiter);
    JsonDocument doc;
    JsonObject obj = doc[str].to<JsonObject>();

    str = strtok(nullptr, delimiter);
    obj["temperature"] = String(str).toFloat();

    str = strtok(nullptr, delimiter);
    obj["humidity"] = String(str).toFloat();

    String json_datapoint;
    serializeJson(doc, json_datapoint);

    // Store JSON buffer datapoint
    File jf = LittleFS.open(json_filepath, FILE_APPEND);
    if (!jf) return true;

    if (!jf.println(json_datapoint)) status = true;

    jf.close();

    return status;
}

bool ClearBuffer() {
    return LittleFS.remove(json_filepath);
}

bool PrintFile(const char* filepath) {
    File f = LittleFS.open(filepath, FILE_READ);
    if (!f) return true;

    while (f.available())
        Serial.write(f.read());

    f.close();
    return false;
}