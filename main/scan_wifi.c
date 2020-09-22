#include "netdb.h"

#include <stdio.h>

#include <esp_wifi.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/FreeRTOSConfig.h"
#include "simple_wifi.h"
#include "include/leach.h"
#include "esp_log.h"
#include "include/consts.h"
#include <inttypes.h>

#define MAX_APs 60
int countRepeat = 0;
int repeat = 1;
char ssidScan[50];

int rssiScan = 0;
int channel = 0;

void scan_wifi(wifi_scan_config_t scan_config);
void next_channel(void);
void getMacAddress(uint8_t baseMac[6]);

void scan_wifi(wifi_scan_config_t scan_config) {
    printf("Começando Escanear Redes Wi-Fi Canal:%d\n",channel);

    esp_wifi_scan_start(&scan_config, true);
    
    uint16_t apCount = 0;

    esp_wifi_scan_get_ap_num(&apCount);
    
    wifi_ap_record_t *list = (wifi_ap_record_t *)malloc(sizeof(wifi_ap_record_t) * apCount);

    esp_wifi_scan_get_ap_records(&apCount,list);

    printf("Total de lista: %d\n",apCount);
 
	for(int i = 0; i < apCount; i++){
        printf("Valor SSID: %s -> Canal: %d -> Valor RSSI:%d\n",(char *) list[i].ssid,list[i].primary,list[i].rssi);
        getMacAddress(list[i].bssid);
        printf("\n");
/*
        if (strcmp((char *) list[i].bssid,MAC_PREFIX_1)  > 3 ) {
            printf("VALOR DO MAC:%s",(char *) list[i].bssid);
        }
        
        if (strcmp((char *) list[i].ssid,ssidScan) == 0 ) {
            rssiScan = list[i].rssi;
            printf("Achou a rede com SSID");
            return;
        }*/

        vTaskDelay(100);
    }

    free(list);
    vTaskDelay(100);

    next_channel();
}


void getMacAddress(uint8_t baseMac[6]) {   
    char baseMacChr[18] = {0};
    sprintf(baseMacChr, "%02X:%02X:%02X:%02X:%02X:%02X", baseMac[0], baseMac[1], baseMac[2], baseMac[3], baseMac[4], baseMac[5]);
    printf("%s",baseMacChr);
    
}


int getCH(const  char *ssid) 
{
    strcpy(ssidScan,ssid);
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();
    ESP_LOGI("Scan Wifi", "Valor eh: %s",ssidScan);
    //global variable to scan_config
    wifi_scan_config_t scan_config = {
		.ssid = 0,
		.bssid = 0,
		.channel = 1,
        .show_hidden = true
    };

    scan_wifi(scan_config);

    free(ssidScan);
    return rssiScan;
}



void next_channel(void) {

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);
    esp_wifi_set_mode(WIFI_MODE_STA);

    esp_wifi_start();
    
    if (channel <= 14)
        channel++;
    else
    {
        esp_wifi_stop();
        vTaskDelay(1000);
        channel = 0;
        esp_wifi_start();
    }

    // configure and run the scan process in blocking mode
	wifi_scan_config_t scan_config = {
		.ssid = 0,
		.bssid = 0,
		.channel = channel,
        .show_hidden = true
    };

    scan_wifi(scan_config);
    
}

void config_wifi(void) {

    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();


    // configure and run the scan process in blocking mode

    //global variable to scan_config
    wifi_scan_config_t scan_config = {
		.ssid = 0,
		.bssid = 0,
		.channel = 1,
        .show_hidden = true
    };

   scan_wifi(scan_config);

}

void setup_wifi(void) {
    wifi_init_config_t wifi_config = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&wifi_config);
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_start();

    //global variable to scan_config
    wifi_scan_config_t scan_config = {
		.ssid = 0,
		.bssid = 0,
		.channel = 1,
        .show_hidden = true
    };

    scan_wifi(scan_config);
}