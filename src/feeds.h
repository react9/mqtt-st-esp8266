void add_pub_feed_key(const char* base, const char* name, uint8_t ix);
char* get_pub_feed_key(uint8_t ix);
void set_pub_feed_key(Adafruit_MQTT_Publish* pub, const char* base, const char* name);
void set_sub_feed_key(Adafruit_MQTT_Subscribe* sub, const char* base, const char* name);
