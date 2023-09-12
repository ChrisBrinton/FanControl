/**
 * @file touch_driver.c
 */

#include "touch_driver.h"
#include "tp_i2c.h"


void touch_driver_init(void)
{

    ft6x06_init(FT6236_I2C_SLAVE_ADDR);

}

#if LVGL_VERSION_MAJOR >= 8
void touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
#else
bool touch_driver_read(lv_indev_drv_t *drv, lv_indev_data_t *data)
#endif
{
    bool res = false;

    res = ft6x36_read(drv, data);

#if LVGL_VERSION_MAJOR >= 8
    data->continue_reading = res;
#else
    return res;
#endif
}
