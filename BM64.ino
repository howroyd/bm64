#include "Bm64_pkt.h"

#define SPP_MAX_PKT_SIZE 128
#define OP_CODE_SPP 0x12

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(1000); // 10s timeout

  Serial.println("Starting");

  Serial1.begin(115200);
  Serial.setTimeout(1000); // 2s timeout

   delay(5000); // Give BM64 time to boot // TODO

  // TODO config BM64

  delay(10);
}

void loop() {
  Serial.println("Tick");
  Pkt_out request_paired_device_list(0x0e);
  request_paired_device_list.send();
  
  delay(1000);
}

void serialEvent()
{
  String str = Serial.readString();
  Serial.println(str);
  Pkt_out pkt_out(0x12, str);
  pkt_out.send();
}

void serialEvent1()
{
  uint8_t BUFFER[SPP_MAX_PKT_SIZE+10];
  uint16_t pkt_len = Serial1.readBytes(BUFFER, SPP_MAX_PKT_SIZE+10);

  Pkt_in pkt(BUFFER, pkt_len);

  Serial.println(pkt.as_string());
}
