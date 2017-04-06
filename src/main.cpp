#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"
#include "main.h"
#include "feeds.h"
#include "utils.h"

#ifdef ENABLE_DHT
#include "dhtm.h"
#endif
#ifdef ENABLE_MCP9808
#include "mcp.h"
#endif
#ifdef ENABLE_ANALOG
#include "an.h"
#endif

/* Constants */
#define ONE_MINUTE (1000UL * 60 * 1)

/* Network state */
WiFiClientSecure client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME,
  AIO_USERNAME, AIO_KEY);

/* Feeds */
Adafruit_MQTT_Publish   pub_dev_cmds = Adafruit_MQTT_Publish(&mqtt, "", MQTT_QOS_1);
Adafruit_MQTT_Subscribe sub_dev_cmds = Adafruit_MQTT_Subscribe(&mqtt, "");

/* Others */

unsigned long rolltime_low = 0;
unsigned long rolltime_high = 0;
unsigned long last_publish_time = 0;
uint8_t poll_index_low = 0;
uint8_t poll_index_high = 0;
uint8_t device = 0;
char esp_serial[16];
char ip[24];

/* Locals */
bool mqtt_connect(Adafruit_MQTT_Publish* pub);
void process_cmds(Adafruit_MQTT_Subscribe s);
void refresh();

/* Setup */
void setup() {

  byte counter = 0;

#ifdef DEBUG_ESP_PORT
  Serial.begin(115200);
  delay(500);
#endif

  DEBUG_MSG(F("\nconnecting to wireless AP"));
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID, WLAN_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    DEBUG_MSG(F("."));
    /* Reset the board after 15 seconds. */
    if(counter > 30) {
      while(1);
    }
    counter++;
  }
  WiFi.localIP().toString().toCharArray(ip, 24);
  DEBUG_MSG(ip);
  DEBUG_MSGLN("");

  /* Set MQTT client id to ESP serial. */
  String(ESP.getChipId(), HEX).toCharArray(esp_serial, 16);
  DEBUG_MSG(F("esp serial id ("));
  DEBUG_MSG(esp_serial);
  DEBUG_MSGLN(F(")"));
  mqtt.setClientid(esp_serial);

  /* Setup feeds. */
  set_pub_feed_key(&pub_dev_cmds, FEED_BASE, "cmds");
  set_sub_feed_key(&sub_dev_cmds, FEED_BASE, "cmds");

  mqtt.subscribe(&sub_dev_cmds);

  /* BOARD SPECIFIED FEEDS. */
  #ifdef ENABLE_DHT
  dht_init();
  add_pub_feed_key(HOME_BASE, "temperature_dht", 1);
  add_pub_feed_key(HOME_BASE, "humidity_dht", 2);

  device |= 0x01;
  #endif
  #ifdef ENABLE_MCP9808
  mcp_init();
  add_pub_feed_key(HOME_BASE, "temperature_mcp", 3);

  device |= 0x02;
  #endif
  #ifdef ENABLE_ANALOG
  an_init();
  add_pub_feed_key(HOME_BASE, "analog", 5);

  device |= 0x16;
  #endif

  /* Setup OTA. */
  ArduinoOTA.setPassword((const char *) OTA_PASSWORD);
  ArduinoOTA.begin();

  /* Start MQTT connection. */
  mqtt.will(sub_dev_cmds.topic, MQTT_DISCONNECTED);
  mqtt_connect(&pub_dev_cmds);

  DEBUG_MSGLN(F("setup complete"));

} /* setup */
/* ----------------------------------------------------------------------- */
/* Loop */
void loop() {

  /* Locals */
  Adafruit_MQTT_Subscribe *subscription;

  /* OTA */
  ArduinoOTA.handle();

  /* Keep retrying if we failed to connect. */
  mqtt_connect(&pub_dev_cmds);

  while ((subscription = mqtt.readSubscription(MQTT_SUB_READ_TIMEOUT))) {
    if ( subscription == &sub_dev_cmds ) {
      process_cmds(sub_dev_cmds);
    }
  }

  if((long)(millis() - rolltime_high >= 0)) {
    rolltime_high += PUBLISH_RATE_HIGH;

    switch (poll_index_high) {
      case 0:
      {
        #ifdef ENABLE_ANALOG
        pub_analog_reading(&mqtt, get_pub_feed_key(5));
        #endif
        poll_index_high++;
      } /* case 0 */
      break;

      default:
      poll_index_high = 0;
      break;
    }

  }

  if((long)(millis() - rolltime_low) >= 0) {
    rolltime_low += PUBLISH_RATE_LOW;
    mqtt.ping();

    switch (poll_index_low) {
      case 0:
      {
        #ifdef ENABLE_DHT
        pub_dht_temperature(&mqtt, get_pub_feed_key(1));
        #endif
        poll_index_low++;
      } /* case 0 */
      break;

      case 1:
      {
        #ifdef ENABLE_DHT
        pub_dht_humidity(&mqtt, get_pub_feed_key(2));
        #endif
        poll_index_low++;
      } /* case 1 */
      break;

      case 2:
      {
        #ifdef ENABLE_MCP9808
        pub_mcp_temperature(&mqtt, get_pub_feed_key(3));
        #endif
        poll_index_low++;
      } /* case 1 */
      break;

      default:
      poll_index_low = 0;
      break;

    } /* switch (poll_index) */
  } /* if((long)(millis() - rolltime_s) >= 0) */
} /* loop */

/* ----------------------------------------------------------------------- */
void process_cmds(Adafruit_MQTT_Subscribe s) {
  DEBUG_MSG(F("received sub cmds: "));
  DEBUG_MSGLN((char *)s.lastread);

  if (strcmp((char *)s.lastread, "reset") == 0) {
    while(1);
  }
  if (strcmp((char *)s.lastread, "refresh") == 0) {
    refresh();
  }
}
/* ----------------------------------------------------------------------- */
void refresh() {
  /* Do all refresh data */
}
/* ----------------------------------------------------------------------- */
bool mqtt_connect(Adafruit_MQTT_Publish* pub) {
  static bool never_connected = false;
  int8_t ret;
  uint8_t retries = 10;

  // Stop if already connected.
  if(never_connected == true)  {
    if (mqtt.connected()) {
      return false;
    }
  }

  DEBUG_MSGLN(F("connecting to mqtt... "));

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected

       // For OTA
       ArduinoOTA.handle();

       DEBUG_MSGLN(mqtt.connectErrorString(ret));
       DEBUG_MSGLN(F("retrying mqtt conection in 5 seconds..."));
       mqtt.disconnect();
       delay(5000);
       retries--;
       if (retries == 0) {
         while (1);
       }
  }

  DEBUG_MSGLN(F("connected to mqtt!"));
  pub->publish(ip);
  pub->publish(device);
  pub->publish(MQTT_CONNECTED);

  never_connected = true;
  return true;
}
/* ----------------------------------------------------------------------- */
