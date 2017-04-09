#ifdef ENABLE_DHT
#include "DHT.h"
void dht_init(Adafruit_MQTT_Client* mqtt, const char* serial, const uint32_t p);
void dht_exec(Adafruit_MQTT_Client* mqtt);
void dht_refresh(Adafruit_MQTT_Client* mqtt);
void dht_pub_humidity(Adafruit_MQTT_Client* mqtt);
void dht_pub_temperature(Adafruit_MQTT_Client* mqtt);
#endif
