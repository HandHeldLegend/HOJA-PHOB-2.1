#ifndef BOARD_CONFIG_H
#define BOARD_CONFIG_H

// Device stuff
#define HOJA_DEVICE_ID  0xF001
#define HOJA_FW_VERSION 0x0A04
#define HOJA_SETTINGS_VERSION 0xA000

// RGB Stuff
//#define HOJA_RGB_PIN 15
//#define HOJA_RGB_COUNT 20
//#define HOJA_RGBW_EN 0

// GPIO definitions
#define HOJA_SERIAL_PIN 28
#define HOJA_CLOCK_PIN 8
#define HOJA_LATCH_PIN 9

// If we do not have native analog triggers
// set this to zero
#define HOJA_ANALOG_TRIGGERS 0
#define HOJA_ANALOG_LIGHT 50

// URL that will display to open a config tool
#define HOJA_WEBUSB_URL     "handheldlegend.github.io/progcc_config"
#define HOJA_MANUFACTURER   "Phob"
#define HOJA_PRODUCT        "Phob-Dev"

#endif
