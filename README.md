# ESP32 WiFi Repeater using Arduino as a component of ESP-IDF 

## Get Started
The following are the steps required to run this project on the ESP32 and turn it into a WiFi repeater.
### Step 1 - Download and setup ESP-IDF
Follow the official setup guide for the ESP-IDF v3.3: https://docs.espressif.com/projects/esp-idf/en/v3.3/get-started/index.html
### Step 2 - Get lwIP library with NAT 
Download the repository of the NAT lwIP library using the following command:

`git clone https://github.com/jonask1337/esp-lwip.git`

Note: It is important to clone the repository. If it is only downloaded it will be replaced by the orginal lwIP library during the build.
### Step 3 - Replace original ESP-IDF lwIP library with NAT lwIP library
1. Go to the folder where the ESP-IDF is installed.
2. Rename or delete the *esp-idf/component/lwip/lwip* directory.
3. Move the lwIP library with NAT repository folder from Step 2 to *esp-idf/component/lwip/*
4. Rename the lwIP library with NAT repository folder from *esp-lwip* to *lwip*.
### Step 4 - Get this project
Clone this repository using the following command:

`git clone https://github.com/jonask1337/esp32-arduino-espidf-comp-wifirepeater.git`
### Step 5 - Setup Arduino as component of ESP-IDF 
Based on instruction from: https://github.com/espressif/arduino-esp32/blob/master/docs/esp-idf_component.md
1. Change into the *esp32-arduino-espidf-comp-wifirepeater* project folder
2. In the project folder, create a folder called components and clone this repository inside

    ```bash
    mkdir -p components && \
    cd components && \
    git clone https://github.com/espressif/arduino-esp32.git arduino && \
    cd arduino && \
    git submodule update --init --recursive && \

### Step 6 - Build and flash project
You should be now able to build the project and flash it to the esp32.

For this either use `idf.py flash` (CMake build system) or `make flash` (make build system).

For more details on building and flashing esp-idf projects see the official *Get Started* guide for ESP-IDF from Step 1.

### Fix Compilation Errors
When building the esp-idf project it may come to some compilation errors due to incompatibilities with components.

#### Error 1
- Error output:
  ```
      /home/jonas/test_arduino_wifirepeater_setup/esp-idf/components/lwip/lwip/src/core/ipv6/nd6.c: 
      In function 'nd6_input': 
      /home/jonas/test_arduino_wifirepeater_setup/esp-idf/components/lwip/lwip/src/core/ipv6/nd6.c:565:39: error: 
      incompatible types when initializing type 'const ip_addr_t * {aka const struct ip_addr *}' 
      using type'ip_addr_t {aka struct ip_addr}' const ip_addr_t *addr = dns_getserver(s);
- Fix:
In file *esp-idf/components/lwip/lwip/src/core/ipv6/nd6.c:565*
   ```C
        ...
        // const ip_addr_t *addr = dns_getserver(s);
        ip_addr_t addr = dns_getserver(s);
        // if(ip_addr_cmp(addr, &rdnss_address)) {
        if(ip_addr_cmp(&addr, &rdnss_address)) {
        ...
   ```
#### Error 2
- Error output:
   ```
        home/jonas/test_arduino_wifirepeater_setup/esp-idf/components/tcpip_adapter/tcpip_adapter_lwip.c: 
        In function 'tcpip_adapter_get_dns_info':
        /home/jonas/test_arduino_wifirepeater_setup/esp-idf/components/tcpip_adapter/tcpip_adapter_lwip.c:797:16: 
        error: incompatible types when assigning to type 'const ip_addr_t * 
        {aka const struct ip_addr *}' from type 'ip_addr_t {aka struct ip_addr}'
        dns_ip = dns_getserver(type);
- Fix:
In file *esp-idf/components/tcpip_adapter/tcpip_adapter_lwip.c:778*
    ```C
        // const ip_addr_t*  dns_ip = NULL;
        ip_addr_t dns_ip;
        ...
        if (tcpip_if == TCPIP_ADAPTER_IF_STA || tcpip_if == TCPIP_ADAPTER_IF_ETH) {
            dns_ip = dns_getserver(type);
            //if (dns_ip != NULL) {
            //    dns->ip = *dns_ip;                                                                             
            //}
            dns->ip = dns_ip;
        ...
    ```
    
## Configuration
In the main/main.cpp file change the values of the *ssid* and *password* variables to the credentials of the WIFI network the ESP32 should connect to.

By Default the DNS-Server which is offerd to clients connecting to the ESP32 AP is set to 8.8.8.8. Replace the value of the MY_DNS_IP_ADDR with your desired DNS-Server IP address (in hex) if you want to use a different one.
