#pragma once
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <Arduino.h>

void InitClock(int DAT, int CLK, int RST);
void SyncClock();
int* GetDateTime();
bool PollSensors();