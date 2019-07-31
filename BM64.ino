size_t string_to_bin(String str, uint8_t* buf);
size_t bin_to_pkt(uint8_t* buf, uint8_t len);


void setup() {
  Serial1.begin(115200);

  delay(10);


  
}

void loop() {

}

size_t string_to_bin(String str, uint8_t* buf)
{
  size_t len = str.length() + 1;
  buf = new uint8_t[len ? len<128 : len=128];
  str.getBytes(buf, len);
  return len;
}

size_t bin_to_pkt(uint8_t* buf, uint8_t len)
{
  
}
