#define X_AXIS_CONFIG 0xD0
#define Y_AXIS_CONFIG 0xF0
#define BUFFER_TO_UINT16(buffer) (uint16_t)(((buffer[0] & 0x07) << 9) | buffer[1] << 1 | buffer[2] >> 7)
#define UINT16_INVERT(val) (uint16_t)(0xFFFF-(val))

#define PGPIO_BTN_A    17
#define PGPIO_BTN_B    16
#define PGPIO_BTN_X    18
#define PGPIO_BTN_Y    19

#define PGPIO_BTN_START 5
#define PGPIO_BTN_L   22
#define PGPIO_BTN_R   21
#define PGPIO_BTN_ZL  12
#define PGPIO_BTN_ZR  20

#define PGPIO_BTN_DUP     9
#define PGPIO_BTN_DDOWN   10
#define PGPIO_BTN_DLEFT   8
#define PGPIO_BTN_DRIGHT  11

// Analog L Trigger ADC
#define PADC_LT 1
// Analog R Trigger ADC
#define PADC_RT 0

#define PGPIO_RUMBLE_MAIN   25
#define PGPIO_RUMBLE_BRAKE  29

// SPI ADC CLK pin
  #define PGPIO_SPI_CLK 6
  // SPI ADC TX pin
  #define PGPIO_SPI_TX  7
  // SPI ADC RX pin
  #define PGPIO_SPI_RX  4

  // Left stick ADC Chip Select
  #define PGPIO_LS_CS   24
  // Right stick ADC Chip Select
  #define PGPIO_RS_CS   23

  //#define PGPIO_IMU0_CS 28
  //#define PGPIO_IMU1_CS 29

  