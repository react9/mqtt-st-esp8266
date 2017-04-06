#ifdef ENABLE_DHT

#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "dhtm.h"
#include "config.h"
#include "utils.h"

static DHT dht(PIN_DHT, DHT_TYPE);

void dht_init() {
  dht.begin();
}

void pub_dht_humidity(Adafruit_MQTT_Client* mqtt, char* feed_key) {
  char fpayload[41];

  DEBUG_MSG("publish dht humidity: "); DEBUG_MSG(feed_key);  DEBUG_MSG(" ");
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
    mqtt->publish(feed_key, fpayload);
  }
}

void pub_dht_temperature(Adafruit_MQTT_Client* mqtt, char* feed_key) {
  char fpayload[41];

  DEBUG_MSG("publish dht temperature: "); DEBUG_MSG(feed_key);  DEBUG_MSG(" ");
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
    mqtt->publish(feed_key, fpayload);
  }
}
#endif
