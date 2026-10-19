#include "include/dht.h"

static DHT* dht = nullptr;

void InitDHT(int datapin) {
    static DHT init(datapin, DHT11);
    dht = &init;
    dht->begin();
}

float* GetSensorVals() {
    static float vals[2];

    vals[0] = dht->readTemperature();
    vals[1] = dht->readHumidity();

    return vals;
}