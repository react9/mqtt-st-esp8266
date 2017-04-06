/* BOARD configuration. */
/* --------------------------------------------*/
//#define ENABLE_DHT        // 0x01 -D ENABLE_DHT
//#define ENABLE_MCP9808    // 0x02 -D ENABLE_MCP9808
//#define ENABLE_CONTACT    // 0x04 -D ENABLE_CONTACT
//#define ENABLE_RELAY      // 0x08 -D ENABLE_RELAY
//#define ENABLE_ANALOG     // 0x16 -D ENABLE_ANALOG
/* --------------------------------------------*/

/* DHT configuration. */
/* --------------------------------------------*/
#define PIN_DHT   D4       /* On the MCP board this is D5 */
#define DHT_TYPE  DHT22
/* --------------------------------------------*/

/* MCP9808 configuration.*/
/* --------------------------------------------*/
/* Use to override defaults. (TBD this doesnt work) */
// #define PIN_MCP9808_SDA D2
// #define PIN_MCP9808_SCL D1
/* --------------------------------------------*/

/* Contact configuration. */
/* --------------------------------------------*/
#define PIN_CONTACTS_TOTAL   1
#define PIN_CONTACT_0       D2
// #define PIN_CONTACT_1    D3
// #define PIN_CONTACT_2    D4
/* --------------------------------------------*/

/* Relay configuration. */
/* --------------------------------------------*/
#define PIN_REAY_TOTAL     1
#define PIN_RELAY_0       D3
// #define PIN_RELAY_1    D4
// #define PIN_RELAY_2    D5
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
#define FIRMWARE_MINOR_REV     1
/* --------------------------------------------*/

#define PUBLISH_RATE_LOW 60000
#define PUBLISH_RATE_HIGH 5000

/* END of user configuration. */
