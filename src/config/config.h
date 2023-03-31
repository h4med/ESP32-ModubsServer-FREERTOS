/**
 * Set this to false to disable Serial logging
 */
#define DEBUG true

/**
 * The name of this device (as defined in the AWS IOT console).
 * Also used to set the hostname on the network
 */
#define DEVICE_NAME "ESP32_Meter"

/**
 * ADC input pin that is used to read out the CT sensor
 */
#define ADC_INPUT 34 // 36 in old config

/**
 * The voltage of your home, used to calculate the wattage.
 * Try setting this as accurately as possible.
 */
#define HOME_VOLTAGE 213.0

/**
 * WiFi credentials   
 */
#define WIFI_NETWORK "------"
#define WIFI_PASSWORD "******"

/**
 * Timeout for the WiFi connection. When this is reached,
 * the ESP goes into deep sleep for 30seconds to try and
 * recover.
 */
#define WIFI_TIMEOUT 20000 // 20 seconds

/**
 * How long should we wait after a failed WiFi connection
 * before trying to set one up again.
 */
#define WIFI_RECOVER_TIME_MS 20000 // 20 seconds