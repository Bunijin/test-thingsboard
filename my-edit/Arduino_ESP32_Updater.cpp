#ifndef ARDUINO_ESP32_UPDATER_H
#define ARDUINO_ESP32_UPDATER_H

#if THINGSBOARD_ENABLE_OTA

#if defined(ESP32) && defined(ARDUINO)

#include <Arduino.h>

namespace ThingsBoard {

    class Arduino_ESP32_Updater {
    public:
        virtual bool begin(const size_t& firmware_size);
        virtual size_t write(uint8_t* payload, const size_t& total_bytes);
        virtual void reset();
        virtual bool end();
    };

} // namespace ThingsBoard

#endif // defined(ESP32) && defined(ARDUINO)

#endif // THINGSBOARD_ENABLE_OTA

#endif // ARDUINO_ESP32_UPDATER_H