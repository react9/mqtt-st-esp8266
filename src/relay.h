#ifdef ENABLE_RELAY
void relay_init(Adafruit_MQTT_Client* mqtt, const char* serial, Adafruit_MQTT_Subscribe* sub);
void relay_exec(Adafruit_MQTT_Client* mqtt);
void relay_refresh(Adafruit_MQTT_Client* mqtt);
void relay_pub_0(Adafruit_MQTT_Client* mqtt);
void relay_process_cmds(Adafruit_MQTT_Client* mqtt, Adafruit_MQTT_Subscribe* subscription);
#endif
