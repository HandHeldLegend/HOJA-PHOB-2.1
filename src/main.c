#include "hoja_includes.h"
#include "app_rumble.h"
#include "app_imu.h"
#include "main.h"


button_remap_s user_map = {
    .dpad_up = MAPCODE_DUP,
    .dpad_down = MAPCODE_DDOWN,
    .dpad_left = MAPCODE_DLEFT,
    .dpad_right = MAPCODE_DRIGHT,

    .button_a = MAPCODE_B_A,
    .button_b = MAPCODE_B_B,
    .button_x = MAPCODE_B_X,
    .button_y = MAPCODE_B_Y,

    .trigger_l = MAPCODE_T_ZL,
    .trigger_r = MAPCODE_T_ZR,
    .trigger_zl = MAPCODE_T_L,
    .trigger_zr = MAPCODE_T_R,

    .button_plus = MAPCODE_B_PLUS,
    .button_minus = MAPCODE_B_MINUS,
    .button_stick_left = MAPCODE_B_STICKL,
    .button_stick_right = MAPCODE_B_STICKR,
};

void cb_hoja_hardware_setup()
{
    // Set up GPIO for input buttons
    hoja_setup_gpio_button(PGPIO_BTN_A);
    hoja_setup_gpio_button(PGPIO_BTN_B);
    hoja_setup_gpio_button(PGPIO_BTN_X);
    hoja_setup_gpio_button(PGPIO_BTN_Y);

    hoja_setup_gpio_button(PGPIO_BTN_START);
    hoja_setup_gpio_button(PGPIO_BTN_L);
    hoja_setup_gpio_button(PGPIO_BTN_R);
    hoja_setup_gpio_button(PGPIO_BTN_ZL);
    hoja_setup_gpio_button(PGPIO_BTN_ZR);

    hoja_setup_gpio_button(PGPIO_BTN_DUP);
    hoja_setup_gpio_button(PGPIO_BTN_DDOWN);
    hoja_setup_gpio_button(PGPIO_BTN_DLEFT);
    hoja_setup_gpio_button(PGPIO_BTN_DRIGHT);

    // initialize SPI at 1 MHz
    // initialize SPI at 3 MHz just to test
    spi_init(spi0, 3000 * 1000);
    gpio_set_function(PGPIO_SPI_CLK, GPIO_FUNC_SPI);
    gpio_set_function(PGPIO_SPI_TX, GPIO_FUNC_SPI);
    gpio_set_function(PGPIO_SPI_RX, GPIO_FUNC_SPI);

    // Left stick initialize
    gpio_init(PGPIO_LS_CS);
    gpio_set_dir(PGPIO_LS_CS, GPIO_OUT);
    gpio_put(PGPIO_LS_CS, true); // active low

    // Right stick initialize
    gpio_init(PGPIO_RS_CS);
    gpio_set_dir(PGPIO_RS_CS, GPIO_OUT);
    gpio_put(PGPIO_RS_CS, true); // active low

    // Set up ADC Triggers
	adc_init();
	adc_gpio_init(PGPIO_LT);
	adc_gpio_init(PGPIO_RT);
}

int lt_offset = 0;
int rt_offset = 0;
bool trigger_offset_obtained = false;

void cb_hoja_read_buttons(button_data_s *data)
{
    // Keypad version
    data->button_a  = !gpio_get(PGPIO_BTN_A);
    data->button_b  = !gpio_get(PGPIO_BTN_B);
    data->button_x  = !gpio_get(PGPIO_BTN_X);
    data->button_y  = !gpio_get(PGPIO_BTN_Y);

    data->dpad_left     = !gpio_get(PGPIO_BTN_DLEFT);
    data->dpad_right    = !gpio_get(PGPIO_BTN_DRIGHT);
    data->dpad_down     = !gpio_get(PGPIO_BTN_DDOWN);
    data->dpad_up       = !gpio_get(PGPIO_BTN_DUP);

    data->button_plus   = !gpio_get(PGPIO_BTN_START);

    data->trigger_r     = !gpio_get(PGPIO_BTN_ZR);
    data->trigger_l     = !gpio_get(PGPIO_BTN_ZL);
    data->trigger_zl    = !gpio_get(PGPIO_BTN_L);
    data->trigger_zr    = !gpio_get(PGPIO_BTN_R);

    // Read Analog triggers
    adc_select_input(PADC_LT);
    int ltr = (int) adc_read();
    adc_select_input(PADC_RT);
    int rtr = (int) adc_read();

    data->zl_analog = ltr;
    data->zr_analog = rtr;
    //data->button_safemode = !gpio_get(PGPIO_BUTTON_MODE);
}

void cb_hoja_read_analog(a_data_s *data)
{
    // Set up buffers for each axis
    uint8_t buffer_lx[3] = {0};
    uint8_t buffer_ly[3] = {0};
    uint8_t buffer_rx[3] = {0};
    uint8_t buffer_ry[3] = {0};

    // CS left stick ADC
    gpio_put(PGPIO_LS_CS, false);
    // Read first axis for left stick
    spi_read_blocking(spi0, X_AXIS_CONFIG, buffer_lx, 3);

    // CS left stick ADC reset
    gpio_put(PGPIO_LS_CS, true);
    gpio_put(PGPIO_LS_CS, false);

    // Set up and read axis for left stick Y  axis
    spi_read_blocking(spi0, Y_AXIS_CONFIG, buffer_ly, 3);

    // CS right stick ADC
    gpio_put(PGPIO_LS_CS, true);
    gpio_put(PGPIO_RS_CS, false);

    spi_read_blocking(spi0, Y_AXIS_CONFIG, buffer_ry, 3);

    // CS right stick ADC reset
    gpio_put(PGPIO_RS_CS, true);
    gpio_put(PGPIO_RS_CS, false);

    spi_read_blocking(spi0, X_AXIS_CONFIG, buffer_rx, 3);

    // Release right stick CS ADC
    gpio_put(PGPIO_RS_CS, true);

    // Convert data
    data->lx = BUFFER_TO_UINT16(buffer_lx);
    data->ly = BUFFER_TO_UINT16(buffer_ly);
    data->rx = BUFFER_TO_UINT16(buffer_rx);
    data->ry = BUFFER_TO_UINT16(buffer_ry);
}

void cb_hoja_task_1_hook(uint32_t timestamp)
{
    app_rumble_task(timestamp);
}

int main()
{
    stdio_init_all();
    sleep_ms(100);

    printf("ProGCC Started.\n");

    hoja_setup_gpio_button(PGPIO_BTN_START);
    // Handle bootloader stuff
    if (!gpio_get(PGPIO_BTN_START))
    {
        reset_usb_boot(0, 0);
    }

    hoja_config_t config = {.input_method = INPUT_METHOD_USB, .input_mode = INPUT_MODE_LOAD};

    hoja_init(&config);
}
