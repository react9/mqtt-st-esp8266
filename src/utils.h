#ifdef DEBUG_ESP_PORT
#define DEBUG_MSG(...) DEBUG_ESP_PORT.print( __VA_ARGS__ )
#define DEBUG_MSGLN(...) DEBUG_ESP_PORT.println( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#define DEBUG_MSGLN(...)
#endif
