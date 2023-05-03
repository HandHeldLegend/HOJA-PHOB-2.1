#include "xinput.h"

short sign_axis(int input)
{
    uint8_t scaled = scale_axis(input);

    int start = (int) scaled - 127;
    if ((start * 256) > 32765)
    {
        start = 32765;
    }
    else if ((start * 256) < -32765)
    {
        start = -32765;
    }
    else
    {
        start *= 256;
    }
    return (short) start;
}

void xinput_hid_report(progcc_button_data_s *button_data, progcc_analog_data_s *analog_data)
{
  if (!tud_xinput_ready()) return;

  static xid_input_s data = {0};

  data.dpad_up = button_data->dpad_up;
  data.dpad_down = button_data->dpad_down;
  data.dpad_left = button_data->dpad_left;
  data.dpad_right = button_data->dpad_right;
  data.button_menu = button_data->button_plus;
  data.bumper_r = button_data->trigger_r;
  data.button_a = button_data->button_a;
  data.button_b = button_data->button_b;
  data.button_x = button_data->button_x;
  data.button_y = button_data->button_y;

  data.analog_trigger_l = button_data->trigger_zl ? 255 : 0;
  data.analog_trigger_r = button_data->trigger_zr ? 255 : 0;

  tud_xinput_report(&data, XID_REPORT_LEN);
}