#include "include/clock.h"

// External clock module variables
static RtcDS1302<ThreeWire>* rtc = nullptr;
RtcDateTime prev;

// Internal clock delay variables
const int period = 5;

void InitClock(int DAT, int CLK, int RST) {
    static ThreeWire wire(DAT, CLK, RST);
    static RtcDS1302<ThreeWire> clock(wire);
    rtc = &clock;
    rtc->Begin();

    if (rtc->GetIsWriteProtected())
        rtc->SetIsWriteProtected(false);

    // Make sure the clock doesn't ruin the non-rechargeable battery
    rtc->SetTrickleChargeSettings(DS1302Tcr_Disabled);

    if (!rtc->GetIsRunning())
        rtc->SetIsRunning(true);
}

// Get and split current datetime
int* GetDateTime() {
    static int dt_arr[6];

    RtcDateTime now = rtc->GetDateTime();

    if (now.IsValid()) {
        prev = now;
        dt_arr[0] = now.Year();
        dt_arr[1] = now.Month();
        dt_arr[2] = now.Day();
        dt_arr[3] = now.Hour();
        dt_arr[4] = now.Minute();
        dt_arr[5] = now.Second();
    }
    else {
        prev += period;  // Add 5 seconds
        dt_arr[0] = prev.Year();
        dt_arr[1] = prev.Month();
        dt_arr[2] = prev.Day();
        dt_arr[3] = prev.Hour();
        dt_arr[4] = prev.Minute();
        dt_arr[5] = prev.Second();
    }

    return dt_arr;
}

bool PollSensors() {
    static unsigned long prev_mils = 0;

    if (millis() - prev_mils >= (period * 1000)) {  // Convert to milliseconds
        prev_mils = millis();
        return true;
    }
    return false;
}