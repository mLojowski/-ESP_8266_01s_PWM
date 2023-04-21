# -ESP_8266_01s_PWM

## Demo code for running:
### - PWM thread
### - Network thread

# TBD:
- add PRINT_DEBUG compilation flag
- phase adjustment
- period control
- duty cycle control
- wifi control
- remoteXY handler

# DONE: 
- PRINT_DEBUG compilation flag - enabled in main/component.mk


# PWM thread:
Simple thread to run PWM signal on 
 - **GPIO0**
 - **GPIO2**
 - **GPIO3**

# Network thread:
Simple example to connect to wifi *(as explained in tutorial ESP8266 WiFi Client)*
using credentials *secret1_SSID* / *secret2_PASS* from `Wifi_credentials.h`

