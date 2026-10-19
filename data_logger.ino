#include "include/clock.h"
#include "include/dht.h"

// Print in csv format
const char format[] = "%04d-%02d-%02d %02d:%02d:%02d,%.1f,%.1f\n";

void setup() {
    Serial.begin(115200);
    InitClock(19, 18, 21);
    InitDHT(4);
}

void loop() {
    if (PollSensors()) {
        int* dt = GetDateTime();
        float* sn = GetSensorVals();

        if (!isnan(sn[0]) && !isnan(sn[1])) 
            Serial.printf(format, dt[0], dt[1], dt[2], dt[3], dt[4], dt[5], sn[0], sn[1]);
    }
}
