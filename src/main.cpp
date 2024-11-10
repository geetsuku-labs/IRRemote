#include <Arduino.h>

#define IR_SEND_PIN 3
#define SEND_PWM_BY_TIMER
#define IR_FEEDBACK_LED_PIN 34

#include <IRremote.hpp>

namespace {
    // Timing chart
    //                             |<---- 1 ---->|<- 0 ->|<- 0 ->|
    //        _____________         ___           ___     ___     ___
    //  _____|             |_______|   |_________|   |___|   |___|   |___...
    //       |<----(1)---->|<-(2)->|(3)|<--(4)-->|(5)|(6)|
    //       |-----Leader Code-----|------Custom Code (40 bits)--------->
    //  (1) HeaderMarkMicros
    //  (2) HeaderSpaceMicros
    //  (3) OneMarkMicros
    //  (4) OneSpaceMicros
    //  (5) ZeroMarkMicros
    //  (6) ZeroSpaceMicros
    struct PulseDistanceWidthProtocolConstants constants_irisohyama =
        {
            PULSE_WIDTH,                       /* Protocol                  */
            38,                                /* FrequencyKHz              */
            {8900, 4400, 550, 1650, 550, 550}, /* DistanceWidthTimingInfo   */
            PROTOCOL_IS_LSB_FIRST,             /* Flags                     */
            0,                                 /* RepeatPeriodMillis        */
            NULL                               /* SpecialSendRepeatFunction */
        };

    enum action_t {
        ACTION_IRIS_POWER,
        ACTION_IRIS_BRIGHTER,
        ACTION_IRIS_DARKER,
        ACTION_MAX
    };

    enum button_events_t {
        // PIN 0-1: UART
        // PIN   2: IR_RECEIVE_PIN
        // PIN   3: IR_SEND_PIN
        // PIN  13: IR_FEEDBACK_LED_PIN
        EVENT_BUTTON_PIN4 = 4,
        EVENT_BUTTON_PIN5,
        EVENT_BUTTON_PIN6,
        EVENT_BUTTON_PIN7,
        EVENT_BUTTON_PIN8,
        EVENT_BUTTON_PIN9,
        EVENT_BUTTON_PIN10,
        EVENT_BUTTON_PIN11,
        EVENT_BUTTON_PIN12,
        EVENT_BUTTON_A0 = PIN_A0,
        EVENT_BUTTON_A1 = PIN_A1,
        EVENT_BUTTON_A2 = PIN_A2,
        EVENT_BUTTON_A3 = PIN_A3,
        EVENT_BUTTON_A4 = PIN_A4,
        EVENT_BUTTON_A5 = PIN_A5,
        EVENT_BUTTON_A6 = PIN_A6,g
        EVENT_BUTTON_A7 = PIN_A7,
        EVENT_MAX
    };

    struct IRAction {
        enum action_t  action;
        IRRawDataType  code_raw[2];
        uint16_t       code_len_bits;
    };

    struct IRAction fm[] = {
        {ACTION_IRIS_POWER,    {0x1207651, 0x5c}, 40,  8},
        {ACTION_IRIS_BRIGHTER, {0x2207651, 0x5f}, 40,  9},
        {ACTION_IRIS_DARKER,   {0x4207651, 0x59}, 40, 10}
    };

    enum functions button2func(uint8_t pin) {
        for (int i=0; i<FUNC_MAX; i++) {
            if (fm[i].pin_button == pin) {
                return fm[i].func;
            }
        }
        return FUNC_MAX;
    }
}

void setup() {
    Serial.begin(115200);
    IrSender.begin();
}

void loop() {
    // Power
    IrSender.sendPulseDistanceWidthFromArray(&constants_irisohyama, fm[FUNC_POWER].code_raw, 40, 0);
    delay(6000);
    IrSender.sendPulseDistanceWidthFromArray(&constants_irisohyama, fm[FUNC_BRIGHTER].code_raw, 40, 0);
    delay(6000);
    IrSender.sendPulseDistanceWidthFromArray(&constants_irisohyama, fm[FUNC_DARKER].code_raw, 40, 0);
    delay(6000);
}
