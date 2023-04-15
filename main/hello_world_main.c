/* Hello World Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/


//#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/pwm.h"



/*

#include "esp_system.h"
#include "esp_spi_flash.h"
#include "driver/uart.h"
*/


//#include "driver/gpio.h"

/* FRTOS WIFI EXAMPLE */
/*
#include "freertos/event_groups.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "nvs_flash.h"
*/

/*
#include "esp8266/gpio_register.h"
#include "esp8266/pin_mux_register.h"
*/

//#ifdef PRINT_DEBUG
#include "esp_log.h"
static const char *TAG = "pwm_example";
//#endif

//SETTINGS FOR PWM DEFINITIONS:

//PINS:
#define PWM_0_OUT_IO_NUM   0
#define PWM_1_OUT_IO_NUM   2
#define PWM_2_OUT_IO_NUM   3    //RX pin


// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD    (10000)

// pwm pin numbers
uint32_t pwm_channels[] = {
    PWM_0_OUT_IO_NUM,       //red
    PWM_1_OUT_IO_NUM,       //gren
    PWM_2_OUT_IO_NUM,       //blue

};

#define pwm_channels_number (sizeof(pwm_channels)/sizeof(pwm_channels[1]))

// duties table, real_duty = duties[x]/PERIOD
uint32_t duties[pwm_channels_number] = {
    10, 10, 10,
};

// phase table, delay = (phase[x]/360)*PERIOD
float phase[pwm_channels_number] = {
    0, 0, 0
};

int setup()
{
    uint8_t rc = 0;

    return rc;
}

void pwm()
{
    pwm_init(PWM_PERIOD, duties, pwm_channels_number, pwm_channels);
    pwm_set_phases(phase);
    pwm_start();
    while (1)
    {
        /* code */
        vTaskDelay(100 / portTICK_RATE_MS);
        
        for (uint8_t channel = 0 ; channel < pwm_channels_number; channel++)
        {
            duties[channel] = (duties[channel] < PWM_PERIOD) ? (duties[channel] + 100) : (duties[channel] % PWM_PERIOD);
            //#ifdef PRINT_DEBUG
            ESP_LOGI(TAG, "PWM Ongoing with duties[%d] = %d!!!\n",channel,duties[channel]);
            //#endif
        }

        pwm_stop(0x0f);
        pwm_set_duties(duties);
        pwm_start();
    }
}


void app_main() 
{
//    setup();

    xTaskCreate(pwm, "PWM", 1024, NULL, 10, NULL);

}
