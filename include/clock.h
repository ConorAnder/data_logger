#pragma once
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <Arduino.h>

void InitClock(int DAT, int CLK, int RST);
int* GetDateTime();
bool PollSensors();