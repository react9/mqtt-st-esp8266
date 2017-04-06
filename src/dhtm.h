#ifdef ENABLE_DHT
#include "DHT.h"
void dht_init();
void pub_dht_humidity(Adafruit_MQTT_Client* mqtt, char* feed_key);
void pub_dht_temperature(Adafruit_MQTT_Client* mqtt, char* feed_key);
#endif
