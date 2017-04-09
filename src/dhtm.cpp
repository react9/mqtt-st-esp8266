#ifdef ENABLE_DHT

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "dhtm.h"
#include "config.h"
#include "utils.h"

#define DHT_KEY_TEMPERATURE "temperature_dht"
#define DHT_KEY_HUMIDITY "humidity_dht"

static DHT dht(PIN_DHT, DHT_TYPE);
static uint32_t poll;
static uint32_t rolltime;
static uint32_t last_publish_time;
static char dht_topic_temperature[128];
static char dht_topic_humidity[128];

void dht_init(Adafruit_MQTT_Client* mqtt, const char* serial, const uint32_t p) {
  poll = p;
  rolltime = 0;
  last_publish_time = 0;

  sprintf(dht_topic_temperature, "%s/%s/%s", HOME_BASE, serial, DHT_KEY_TEMPERATURE);
  sprintf(dht_topic_humidity, "%s/%s/%s", HOME_BASE, serial, DHT_KEY_HUMIDITY);

  dht.begin();
}

void dht_exec(Adafruit_MQTT_Client* mqtt) {
  static uint8_t c;

  if ((int32_t)(millis() - rolltime) >= 0) {
    rolltime += poll;
    if( (c % 2) > 0) {
      dht_pub_humidity(mqtt);
    }
    else {
      dht_pub_temperature(mqtt);
    }

    c++;
  }
}

void dht_refresh(Adafruit_MQTT_Client* mqtt) {
  dht_pub_humidity(mqtt);
  dht_pub_temperature(mqtt);
}

void dht_pub_humidity(Adafruit_MQTT_Client* mqtt) {
  char fpayload[41];

  DEBUG_MSG("publish dht humidity: "); DEBUG_MSG(dht_topic_humidity);  DEBUG_MSG(" ");
  float f;
  f = dht.readHumidity();
  if(isnan(f)) {
    DEBUG_MSGLN("(failed to read DHT humidity - trying again)");
    delay(10);
    f = dht.readHumidity();
  }

  if(isnan(f)) {
    DEBUG_MSGLN("(failed to read DHT humidity)");
  }
  else {
    dtostrf(f, 0, 2, fpayload);
    DEBUG_MSGLN(fpayload);
    mqtt->publish(dht_topic_humidity, fpayload);
  }
}

void dht_pub_temperature(Adafruit_MQTT_Client* mqtt) {
  char fpayload[41];

  DEBUG_MSG("publish dht temperature: "); DEBUG_MSG(dht_topic_temperature);  DEBUG_MSG(" ");
  float f;
  f = dht.readTemperature();
  if(isnan(f)) {
    DEBUG_MSGLN("(failed to read DHT temperature - trying again)");
    delay(10);
    f = dht.readTemperature();
  }

  if(isnan(f)) {
    DEBUG_MSGLN("(failed to read DHT temperature)");
  }
  else {
    dtostrf(f, 0, 2, fpayload);
    DEBUG_MSGLN(fpayload);
    mqtt->publish(dht_topic_temperature, fpayload);
  }
}
#endif
