#ifdef ENABLE_CONTACT
void contact_init(Adafruit_MQTT_Client* mqtt, const char* serial);
void contact_exec(Adafruit_MQTT_Client* mqtt);
void contact_refresh(Adafruit_MQTT_Client* mqtt);
void contact_pub_0(Adafruit_MQTT_Client* mqtt, uint8_t force);
#endif
