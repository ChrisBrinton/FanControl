#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_freertos_hooks.h"
#include "freertos/semphr.h"
#include "esp_system.h"
#include "driver/gpio.h"

#include <lvgl.h>
#include <lvgl_helpers.h>

/*********************
 *      DEFINES
 *********************/
#define TAG "FanControl"
#define LV_TICK_PERIOD_MS 1
#define TFT_CS  5
#define TFT_DC  2
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_LED 4 
#define TFT_RST 27 
#define TFT_MISO -1 


/**********************
 *  STATIC PROTOTYPES
 **********************/
static void lv_tick_task(void *arg);
static void guiTask(void *pvParameter);

static void event_handler(lv_obj_t * obj, lv_event_t event)
{
    if(event == LV_EVENT_CLICKED) {
        printf("Clicked\n");
    }
    else if(event == LV_EVENT_VALUE_CHANGED) {
        printf("Toggled\n");
    }
}

void lv_ex_btn_1(void)
{
    lv_obj_t * label;

    lv_obj_t * btn_preset_1 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_preset_1, event_handler);
    lv_obj_align(btn_preset_1, NULL, LV_ALIGN_CENTER, 140, -40);
    lv_btn_set_checkable(btn_preset_1, true);
    lv_btn_toggle(btn_preset_1);
    lv_btn_set_fit2(btn_preset_1, LV_FIT_NONE, LV_FIT_TIGHT);

    label = lv_label_create(btn_preset_1, NULL);
    lv_label_set_text(label, "PLA");

    lv_obj_t * btn_preset_2 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_preset_2, event_handler);
    lv_obj_align(btn_preset_2, NULL, LV_ALIGN_CENTER, 140, 30);
    lv_btn_set_checkable(btn_preset_2, true);
    //lv_btn_toggle(btn_preset_2);
    lv_btn_set_fit2(btn_preset_2, LV_FIT_NONE, LV_FIT_TIGHT);

    label = lv_label_create(btn_preset_2, NULL);
    lv_label_set_text(label, "PETG");

    lv_obj_t * btn_preset_3 = lv_btn_create(lv_scr_act(), NULL);
    lv_obj_set_event_cb(btn_preset_3, event_handler);
    lv_obj_align(btn_preset_3, NULL, LV_ALIGN_CENTER, 140, 100);
    lv_btn_set_checkable(btn_preset_3, true);
    //lv_btn_toggle(btn_preset_3);
    lv_btn_set_fit2(btn_preset_3, LV_FIT_NONE, LV_FIT_TIGHT);

    label = lv_label_create(btn_preset_3, NULL);
    lv_label_set_text(label, "ABS");
}

/**********************
 *   APPLICATION MAIN
 **********************/
extern "C" void app_main() {

    /* If you want to use a task to create the graphic, you NEED to create a Pinned task
     * Otherwise there can be problem such as memory corruption and so on.
     * NOTE: When not using Wi-Fi nor Bluetooth you can pin the guiTask to core 0 */
    xTaskCreatePinnedToCore(guiTask, "gui", 4096*2, NULL, 0, NULL, 0);
}

/* Creates a semaphore to handle concurrent call to lvgl stuff
 * If you wish to call *any* lvgl function from other threads/tasks
 * you should lock on the very same semaphore! */
SemaphoreHandle_t xGuiSemaphore;

static void guiTask(void *pvParameter) {

    (void) pvParameter;
    xGuiSemaphore = xSemaphoreCreateMutex();

    lv_init();

    /* Initialize SPI or I2C bus used by the drivers */
    lvgl_driver_init();

    lv_color_t* buf1 = (lv_color_t*)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf1 != NULL);

    /* Use double buffering */
    lv_color_t* buf2 = (lv_color_t*)heap_caps_malloc(DISP_BUF_SIZE * sizeof(lv_color_t), MALLOC_CAP_DMA);
    assert(buf2 != NULL);

    static lv_disp_buf_t disp_buf;

    uint32_t size_in_px = DISP_BUF_SIZE;

    /* Initialize the working buffer depending on the selected display.
     * NOTE: buf2 == NULL when using monochrome displays. */
    lv_disp_buf_init(&disp_buf, buf1, buf2, size_in_px);

    lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.flush_cb = disp_driver_flush;

    disp_drv.buffer = &disp_buf;
    lv_disp_drv_register(&disp_drv);

    /* Register an input device*/
    lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);
    indev_drv.read_cb = touch_driver_read;
    indev_drv.type = LV_INDEV_TYPE_POINTER;
    lv_indev_drv_register(&indev_drv);

    /* Create and start a periodic timer interrupt to call lv_tick_inc */
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &lv_tick_task,
        .name = "periodic_gui"
    };
    
    esp_timer_handle_t periodic_timer;
    ESP_ERROR_CHECK(esp_timer_create(&periodic_timer_args, &periodic_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(periodic_timer, LV_TICK_PERIOD_MS * 1000));

    /* Create the application */
    lv_ex_btn_1();

    while (1) {
        /* Delay 1 tick (assumes FreeRTOS tick is 10ms */
        vTaskDelay(pdMS_TO_TICKS(10));

        /* Try to take the semaphore, call lvgl related function on success */
        if (pdTRUE == xSemaphoreTake(xGuiSemaphore, portMAX_DELAY)) {
            lv_task_handler();
            xSemaphoreGive(xGuiSemaphore);
       }
    }

    /* A task should NEVER return */
    free(buf1);
    free(buf2);
    vTaskDelete(NULL);
}

static void lv_tick_task(void *arg) {
    (void) arg;

    lv_tick_inc(LV_TICK_PERIOD_MS);
}
