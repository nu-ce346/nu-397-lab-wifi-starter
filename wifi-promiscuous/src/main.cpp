#include <arduino.h> // Required for all code
#include <wifi.h> // Required for all wifi code
#include <esp_wifi.h>
#include <esp_wifi_types.h>

typedef struct {
  unsigned frame_ctrl:16;
  unsigned duration_id:16;
  uint8_t addr1[6]; /* receiver address */
  uint8_t addr2[6]; /* sender address */
  uint8_t addr3[6]; /* filtering address */
  unsigned sequence_ctrl:16;
  uint8_t addr4[6]; /* optional */
} wifi_ieee80211_mac_hdr_t;

typedef struct {
  wifi_ieee80211_mac_hdr_t hdr;
  uint8_t payload[0]; /* network data ended with 4 bytes csum (CRC32) */
} wifi_ieee80211_packet_t;

const char* wifi_sniffer_packet_type2str(wifi_promiscuous_pkt_type_t type)
{
  switch(type) {
    case WIFI_PKT_MGMT: return "MGMT";
    case WIFI_PKT_DATA: return "DATA";
    default:  
      case WIFI_PKT_MISC: return "MISC";
  }
}

void sniffer_callback(void* buf, wifi_promiscuous_pkt_type_t type) {
    // data!
    const wifi_promiscuous_pkt_t* raw = (wifi_promiscuous_pkt_t*)buf;
    const wifi_ieee80211_packet_t* pkt = (wifi_ieee80211_packet_t*)raw->payload;
    const wifi_ieee80211_mac_hdr_t* hdr = &(pkt->hdr);

    // TODO
}


void setup()
{
    Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was previously connected.
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // Pick the channel to scan on
    // Set to channel 1 by default, you should change this though
    esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

    // Start promiscuous scanning. Callback function will be called for each packet received
    esp_wifi_set_promiscuous_rx_cb(&sniffer_callback); 
    esp_wifi_set_promiscuous(true); 

    Serial.println("Setup done");
}

void loop()
{
    // Nothing here
}

