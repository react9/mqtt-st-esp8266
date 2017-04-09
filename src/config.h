/* BOARD configuration. -- Customize via platformio.ini environments per board */
/* --------------------------------------------*/
//#define ENABLE_DHT
//#define PIN_DHT D5
//#define DHT_TYPE  DHT22

//#define ENABLE_MCP9808
//#define PIN_MCP9808_SDA D2 (NOT WORKING)
//#define PIN_MCP9808_SCL D1 (NOT WORKING)

//#define ENABLE_CONTACT
//#define PIN_CONTACT_0 D2

//#define ENABLE_RELAY
//#define PIN_RELAY_0 D1

//#define ENABLE_ANALOG

//#define ENABLE_RGB
//#define PIN_RGB D3
//#define PIN_RGB_TOTAL_LEDS   32
/* --------------------------------------------*/

/* WiFi configuration. */
/* --------------------------------------------*/
#define WLAN_SSID                "ssid"
#define WLAN_PASS              "secret"
#define OTA_PASSWORD              "xyz"
/* --------------------------------------------*/

/* MQTT configuration. */
/* --------------------------------------------*/
#define AIO_SERVER        "localhost"
#define AIO_SERVERPORT    8883
#define AIO_USERNAME      "user"
#define AIO_KEY           "passsword"
#define FEED_BASE         "feeds" // i.e., feeds/serial#/cmds
#define HOME_BASE         "home" // i.e., home/serial#/sensor
/* --------------------------------------------*/

/* SYSTEM configuration. */
/* --------------------------------------------*/
#define FIRMWARE_MAJOR_REV     0
#define FIRMWARE_MINOR_REV     2
/* --------------------------------------------*/

/* END of user configuration. */
