#include "Arduino.h"
#include "esp_wifi.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include "esp_log.h"
#include "lwip/lwip_napt.h"

const char* ssid = "WIFI_SSID";
const char* password = "WIFI_AP";

#define MY_DNS_IP_ADDR 0x08080808 // 8.8.8.8

WebServer server(80);


void setup(){    
    Serial.begin(115200);    
    WiFi.mode(WIFI_AP_STA);
    WiFi.softAP("esp32-ap","test1234",6);
    WiFi.begin(ssid, password);
    Serial.println("");
    
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED) {
	delay(500);
	Serial.print(".");
    }
    
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    Serial.println(WiFi.dnsIP());
    WiFi.config(WiFi.localIP(), WiFi.gatewayIP(), WiFi.subnetMask(), IPAddress(8,8,8,8)); 
    delay(10);
    Serial.println(WiFi.dnsIP());
   
    // Enable Napt
#if IP_NAPT
    u32_t napt_netif_ip = 0xC0A80401; // Set to ip address of softAP netif (Default is 192.168.4.1)
    ip_napt_enable(htonl(napt_netif_ip), 1);
    Serial.println("NAT is enabled");
#endif

}

void loop(){
    Serial.println("Arduino world");
    delay(1000);
}
