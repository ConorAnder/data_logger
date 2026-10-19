#include "include/wifi.h"

WiFiManager wm;
static WebServer server(80);

void InitWifi() {
    wm.setConfigPortalBlocking(false);
    wm.autoConnect("temp_humidity_logger");

    server.on("/", []() {
        server.send(200, "text/plain", "Temperature and Humidity Logger");
    });

    server.on("/download", HTTP_GET, DownloadCSV);

    server.begin();
}

void UpdateWifi() {
    wm.process();
    server.handleClient();
}

void DownloadCSV() {
    File f = LittleFS.open("/datapoints.csv", FILE_READ);

    if (!f) {
        server.send(404, "text/plain", "CSV file not found");
        return;
    }

    server.sendHeader("Content-Disposition", "attachment; filename=\"datapoints.csv\"");
    server.streamFile(f, "text/csv");

    f.close();
}