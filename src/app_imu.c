#include "interval.h"
#include "hoja_includes.h"
#include "main.h"

void cb_hoja_read_imu(imu_data_s *data_a, imu_data_s *data_b)
{
    data_a->gx = 0;
    data_a->gy = 0;
    data_a->gz = 0;
    data_a->ax = 0;
    data_a->ay = 0;
    data_a->az = 0;

    data_b->gx = 0;
    data_b->gy = 0;
    data_b->gz = 0;
    data_b->ax = 0;
    data_b->ay = 0;
    data_b->az = 0;
}
