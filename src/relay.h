#ifdef ENABLE_RELAY
void relay_init(Adafruit_MQTT_Client* mqtt, const char* serial);
void relay_exec(Adafruit_MQTT_Client* mqtt);
void relay_refresh(Adafruit_MQTT_Client* mqtt);
void relay_pub_0(Adafruit_MQTT_Client* mqtt);
void relay_process_cnds(Adafruit_MQTT_Client* mqtt, Adafruit_MQTT_Subscribe* subscription);
#endif
