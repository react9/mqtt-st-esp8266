#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"
#include "main.h"
#include "utils.h"

#ifdef ENABLE_DHT
#include "dhtm.h"
#endif
#ifdef ENABLE_MCP9808
#include "mcp.h"
#endif
#ifdef ENABLE_CONTACT
#include "contact.h"
#endif
#ifdef ENABLE_ANALOG
#include "an.h"
#endif
#ifdef ENABLE_RGB
#include "rgb.h"
#endif
#ifdef ENABLE_RELAY
#include "relay.h"
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
char default_pub_topic[128];
char default_sub_topic[128];
bool setup_complete = false;
char esp_serial[16];
char ip[24];

/* Locals */
bool mqtt_connect(Adafruit_MQTT_Publish* pub);
void pub_onconnect_msges(Adafruit_MQTT_Client* mqtt);
void process_cmds(Adafruit_MQTT_Subscribe s);
void refresh(Adafruit_MQTT_Client* mqtt);

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
  sprintf(default_pub_topic, "%s/%s/%s", HOME_BASE, esp_serial, "cmds");
  sprintf(default_sub_topic, "%s/%s/%s", FEED_BASE, esp_serial, "cmds");
  pub_dev_cmds.setFeed(default_pub_topic);
  sub_dev_cmds.setFeed(default_sub_topic);

  mqtt.subscribe(&sub_dev_cmds);

  /* BOARD SPECIFIED FEEDS. */
  #ifdef ENABLE_DHT
  device |= 0x01;
  dht_init(&mqtt, esp_serial, 60000);
  #endif

  #ifdef ENABLE_MCP9808
  device |= 0x02;
  mcp_init(&mqtt, esp_serial, 60000);
  #endif

  #ifdef ENABLE_CONTACT
  device |= 0x4;
  contact_init(&mqtt, esp_serial);
  #endif

  #ifdef ENABLE_ANALOG
  device |= 0x8;
  an_init(&mqtt, esp_serial, 5000);
  #endif

  #ifdef ENABLE_RELAY
  device |= 0x16;
  relay_init(&mqtt, esp_serial);
  #endif

  #ifdef ENABLE_RGB
  device |= 0x32;
  rgb_init(&mqtt, esp_serial);
  #endif

  /* Setup OTA. */
  ArduinoOTA.setPassword((const char *) OTA_PASSWORD);
  ArduinoOTA.begin();

  /* Start MQTT connection. */
  mqtt.will(sub_dev_cmds.topic, MQTT_DISCONNECTED);
  mqtt_connect(&pub_dev_cmds);

  setup_complete = true;

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
  if(mqtt_connect(&pub_dev_cmds) || setup_complete) {
    pub_onconnect_msges(&mqtt);
  }

  while ((subscription = mqtt.readSubscription(MQTT_SUB_READ_TIMEOUT))) {
    if ( subscription == &sub_dev_cmds ) {
      process_cmds(sub_dev_cmds);
    }
    else {
      #ifdef ENABLE_RGB
      rgb_process_cnds(&mqtt, subscription);
      #endif
      #ifdef ENABLE_RELAY
      relay_process_cnds(&mqtt, subscription);
      #endif
    }
  }

  /* Run all modules execs */
  #ifdef ENABLE_DHT
  dht_exec(&mqtt);
  #endif

  #ifdef ENABLE_MCP9808
  mcp_exec(&mqtt);
  #endif

  #ifdef ENABLE_CONTACT
  contact_exec(&mqtt);
  #endif

  #ifdef ENABLE_ANALOG
  an_exec(&mqtt);
  #endif

  #ifdef ENABLE_RGB
  rgb_exec(&mqtt);
  #endif

  #ifdef ENABLE_RELAY
  relay_exec(&mqtt);
  #endif

} /* loop */

/* ----------------------------------------------------------------------- */
void process_cmds(Adafruit_MQTT_Subscribe s) {
  DEBUG_MSG(F("received sub cmds: "));
  DEBUG_MSGLN((char *)s.lastread);

  if (strcmp((char *)s.lastread, "reset") == 0) {
    while(1);
  }
  if (strcmp((char *)s.lastread, "refresh") == 0) {
    refresh(&mqtt);
  }

}
/* ----------------------------------------------------------------------- */
void refresh(Adafruit_MQTT_Client* mqtt) {
  #ifdef ENABLE_DHT
  dht_refresh(mqtt);
  #endif

  #ifdef ENABLE_MCP9808
  mcp_refresh(mqtt);
  #endif

  #ifdef ENABLE_CONTACT
  contact_refresh(mqtt);
  #endif

  #ifdef ENABLE_ANALOG
  an_refresh(mqtt);
  #endif

  #ifdef ENABLE_RGB
  rgb_refresh(mqtt);
  #endif

  #ifdef ENABLE_RELAY
  relay_refresh(mqtt);
  #endif
}
/* ----------------------------------------------------------------------- */
void pub_onconnect_msges(Adafruit_MQTT_Client* mqtt) {
  /* for the future */
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
