#ifdef ENABLE_ANALOG
void an_init(Adafruit_MQTT_Client* mqtt, const char* serial, const uint32_t p);
void an_exec(Adafruit_MQTT_Client* mqtt);
void an_refresh(Adafruit_MQTT_Client* mqtt);
void an_pub_0(Adafruit_MQTT_Client* mqtt);
#endif
