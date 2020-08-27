#ifndef TCP_SERVER_INCLUDED
#define TCP_SERVER_INCLUDED

#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event_loop.h"

#ifdef __cplusplus
extern "C" {
#endif

void initialise_wifi(void);
void wait_for_ip();
void tcp_server_task(void *pvParameters);


    #ifdef __cplusplus
    }
    #endif
#endif