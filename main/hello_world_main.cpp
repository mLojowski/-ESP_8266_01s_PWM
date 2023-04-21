/* Hello World Example

    Basic PWM configuration redone from:
        /ESP8266_RTOS_SDK/examples/get-started/hello_world

*/

//Example class to confirm CPP compilation
class Rectangle {
    int width, height;
  public:
    void set_values (int,int);
    int area (void);
} rect;


//#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//PWM include
#include "driver/pwm.h"

//WiFi include
#include "esp_system.h"
#include "driver/uart.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"



#ifdef PRINT_DEBUG
#include "esp_log.h"
const char *TAG = "Pwm_example";
uint16_t dat=0;
#endif


//SETTINGS FOR PWM DEFINITIONS:
//PINS:
#define PWM_0_OUT_IO_NUM   0
#define PWM_1_OUT_IO_NUM   2
#define PWM_2_OUT_IO_NUM   3    //RX pin

// PWM period 1000us(1Khz), same as depth
#define PWM_PERIOD    (1000)

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



//SETTING FOR WiFi DEFINITIONS:
#include "Wifi_credentials.h"



int setup()     //Re think the setup concept
{
    uint8_t rc = 0;
    return rc;
}



//Thread based functions:
extern "C" void pwm(void *)
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
            duties[channel] = (duties[channel] < PWM_PERIOD) ? (duties[channel] + 10) : (duties[channel] % PWM_PERIOD);
            #ifdef PRINT_DEBUG
                ESP_LOGI(TAG, "%s PWM Ongoing with duties[%d] = %d!!!", __func__, channel,duties[channel]);
            #endif
        }

        pwm_stop(0x0f);
        pwm_set_duties(duties);
        pwm_start();
        
        #ifdef PRINT_DEBUG
            dat = uxTaskGetStackHighWaterMark(NULL);
            ESP_LOGI(TAG,"%s stack left: %d", __func__,dat);

        #endif
    }
}

extern "C" void wifi(void *)
{
    nvs_flash_init();
    tcpip_adapter_init();
    esp_event_loop_create_default();
    
    wifi_init_config_t config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&config);

    wifi_config_t net_conf = {
        .sta = {
            {.ssid = SSID},
            {.password = PASSWD}
        },
    };

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(ESP_IF_WIFI_STA, &net_conf);
    
    esp_wifi_start();
    esp_wifi_connect();

    while (1)
    {   //For unknown reason whithout this delay tcpip_adapter is unable to obtain IP 
        // In result of that wdt reset occures after timeout
        vTaskDelay(100 / portTICK_RATE_MS);

        #ifdef PRINT_DEBUG
            dat = uxTaskGetStackHighWaterMark(NULL);
            ESP_LOGI(TAG,"%s stack left: %d", __func__,dat);
        #endif
    }
}




extern "C" void app_main() 
{
//    setup();


    xTaskCreate(pwm, "PWM", 1024, NULL, 10, NULL);
    xTaskCreate(wifi, "WIFI", 1536, NULL, 10, NULL);


}
