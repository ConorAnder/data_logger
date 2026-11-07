#include "include/database.h"

// Database and authentication
static UserAuth* user_auth = nullptr;
FirebaseApp app;
WiFiClientSecure ssl_client;
using AsyncClient = AsyncClientClass;
AsyncClient aClient(ssl_client);
RealtimeDatabase Database;

// Internal clock delay variables
const int period = 300;

void InitDatabase() {
    user_auth = new UserAuth(api, email, pass);
    ssl_client.setInsecure();
    ssl_client.setConnectionTimeout(1000);
    ssl_client.setHandshakeTimeout(5);

    initializeApp(aClient, app, getAuth(*user_auth), processData, "🔐 authTask");
    app.getApp<RealtimeDatabase>(Database);
    Database.url(url);
}

bool SendData() {
    if (!ShouldSendData() || !app.ready()) return false;

    File f = LittleFS.open(json_filepath, FILE_READ);
    if (!f) return false;

    // Convert JSONL doc to JSON for firebase
    JsonDocument batch;
    JsonObject root = batch.to<JsonObject>(); // Ref to root of batch

    while (f.available()) {
        String line = f.readStringUntil('\n');
        line.trim();

        if (line.isEmpty())
            continue;

        JsonDocument reading;

        if (deserializeJson(reading, line))
            continue;

        JsonObject obj = reading.as<JsonObject>();

        for (JsonPair kv : obj) {
            root[kv.key().c_str()].set(kv.value());
        }
    }
    f.close();

    // Send JSON to database
    String msg;
    serializeJson(batch, msg);
    Serial.println(msg);

    return Database.update<object_t>(aClient, DB_FILEPATH, object_t(msg));
}

bool ShouldSendData() {
    static unsigned long prev_mils = 0;

    if (millis() - prev_mils >= (period * 1000)) {  // Convert to milliseconds
        prev_mils = millis();
        return true;
    }
    return false;
}

void processData(AsyncResult &aResult) {
    // Exits when no result is available when calling from the loop.
    if (!aResult.isResult())
        return;

    if (aResult.isEvent())
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.eventLog().message().c_str(), aResult.eventLog().code());

    if (aResult.isDebug())
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());

    if (aResult.isError())
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());

    if (aResult.available())
        Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
}

void UpdateApp() {
    app.loop();
}