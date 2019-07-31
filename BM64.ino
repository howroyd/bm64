#include "Bm64_pkt.h"

#define SPP_MAX_PKT_SIZE 128
#define OP_CODE_SPP 0x12

void setup() {
  Serial.begin(115200);
  Serial.setTimeout(10000); // 10s timeout

  Serial1.begin(115200);
  Serial.setTimeout(2000); // 2s timeout

  // TODO config BM64

  delay(10);
}

void loop() {
  delay(1000);
}

void serialEvent()
{
  String str = Serial.readString();

  BM64_uart_pkt_t pkt;
  pkt.payload_len = str.length()+1;
  pkt.op_code = 0x12;

  pkt.payload = new uint8_t[pkt.payload_len];
  str.getBytes(pkt.payload, pkt.payload_len);

  Pkt_out pkt_out(pkt);
  pkt_out.send();
}

void serialEvent1()
{
  uint8_t BUFFER[SPP_MAX_PKT_SIZE+10];
  uint16_t pkt_len = Serial1.readBytes(BUFFER, SPP_MAX_PKT_SIZE+10);

  Pkt_in pkt(BUFFER, pkt_len);

  Serial.println(pkt.as_string());
}
