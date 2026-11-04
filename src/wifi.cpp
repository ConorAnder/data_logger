#include "include/wifi.h"

static WiFiManager wm;

WebServer server(80);

char url[TOKEN_LENGTH];
char api[TOKEN_LENGTH];
char email[TOKEN_LENGTH];
char pass[TOKEN_LENGTH];
static WiFiManagerParameter wm_firebase_url("url", "Database URL", url, TOKEN_LENGTH);
static WiFiManagerParameter wm_firebase_key("key", "API key", api, TOKEN_LENGTH);
static WiFiManagerParameter wm_firebase_email("email", "User email", email, TOKEN_LENGTH);
static WiFiManagerParameter wm_firebase_pass("pass", "User password", pass, TOKEN_LENGTH, "type=password");

void InitWifi() {
    // Initialise database connection parameters
    Preferences pr;
    if (pr.begin("firebase", true)) {
        strcpy(api, pr.getString("api_key", "").c_str());
        strcpy(email, pr.getString("email", "").c_str());
        strcpy(pass, pr.getString("password", "").c_str());
        pr.end();
    }
    
    wm_firebase_url.setValue(url, TOKEN_LENGTH);
    wm_firebase_key.setValue(api, TOKEN_LENGTH);
    wm_firebase_email.setValue(email, TOKEN_LENGTH);
    wm_firebase_pass.setValue(pass, TOKEN_LENGTH);

    wm.addParameter(&wm_firebase_url);
    wm.addParameter(&wm_firebase_key);
    wm.addParameter(&wm_firebase_email);
    wm.addParameter(&wm_firebase_pass);

    wm.setSaveParamsCallback(UpdateSetParams);

    // Initialise wifi connection
    wm.setConfigPortalBlocking(false);
    wm.setBreakAfterConfig(true);
    wm.setHttpPort(8080);                   // Datalogger server runs on port 80 so we need this to run in a different port
    wm.autoConnect("temp_humidity_logger");

    // Initialise wifi server to control config remotely
    server.on("/", []() {
        server.send(200, "text/plain", "Temperature and Humidity Logger");
    });

    server.on("/download", HTTP_GET, DownloadCSV);

    server.on("/config", HTTP_GET, ConfigServerParams);

    server.on("/clear", HTTP_POST, []() {
        if (FormatFileStorage()) {
            server.send(500, "text/plain", "Failed to format storage");
            return;
        }

        server.send(200, "text/plain", "Storage cleared successfully");
    });

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

void ConfigServerParams() {
    if (!wm.getWebPortalActive()) {
        wm.startWebPortal();
        Serial.println("Opening config portal...");
        while (!wm.getWebPortalActive());
        Serial.println("Web portal open!");
    }
}

void UpdateSetParams() {
    // Pull updated credentials from config portal
    strcpy(url, wm_firebase_url.getValue());
    strcpy(api, wm_firebase_key.getValue());
    strcpy(email, wm_firebase_email.getValue());
    strcpy(pass, wm_firebase_pass.getValue());

    // Save updated credentials to permanent storage
    Preferences pr;
    if (!pr.begin("firebase", false)) {
        Serial.println("Failed to open saved parameters");
        return;
    }

    if (
        pr.putString("url", url) == 0 ||
        pr.putString("api_key", api) == 0 ||
        pr.putString("email", email) == 0 ||
        pr.putString("password", pass) == 0
    ) {
        Serial.println("Failed to save parameters");
    }

    pr.end();
    Serial.println("Parameters updated!");
}