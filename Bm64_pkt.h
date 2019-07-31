typedef struct
{
  uint8_t sinc = 0xAA;
  uint16_t pkt_len = 1;
  uint8_t op_code;
  uint8_t* payload;
  uint8_t payload_len;
  uint8_t chk;
} BM64_uart_pkt_t;

class Pkt_in
{
public:
  Pkt_in(uint8_t* buf, uint16_t pkt_len)
  {
    pkt.sinc = *buf;
    pkt.pkt_len = *((uint16_t*)(buf+2));
    pkt.op_code = *buf+3;
    delete pkt.payload;
    pkt.payload = new uint8_t[pkt_len-4];
    memcpy((void*)pkt.payload, (void*)(buf+4), sizeof(uint8_t)*(pkt_len-5));
    pkt.chk = *(buf+pkt_len-1);
  }

  String as_string()
  {
    return String((char*)pkt.payload);
  }
  
  BM64_uart_pkt_t pkt;
};

class Pkt_out
{
  public:
    Pkt_out(BM64_uart_pkt_t& pkt)
    {
      this->pkt = pkt;
      update_checksum();
    }
    Pkt_out(const uint8_t op_code)
    {
      pkt.op_code = op_code;
      pkt.pkt_len;
      update_checksum();
    }
    Pkt_out(const uint8_t op_code, String payload)
    {
      pkt.op_code = op_code;
      pkt.payload_len = payload.length()+1;
      pkt.pkt_len += pkt.payload_len;
      pkt.payload = new uint8_t[pkt.payload_len];
      payload.getBytes(pkt.payload, pkt.payload_len);
      update_checksum();
    }
    ~Pkt_out()
    {
      delete pkt.payload;
      delete _pkt_bin;
    }

    void send()
    {
      format();
      Serial1.write(_pkt_bin, _pkt_len);
    }

    void format()
    {
      uint8_t* ptr = _pkt_bin;
      
      _pkt_len = pkt.pkt_len + 4;
      
      if (_pkt_bin != nullptr)
      {
        delete _pkt_bin;
      }
      
      _pkt_bin = new uint8_t[_pkt_len];
      
      memcpy((void*)(_pkt_bin), (void*)&pkt.sinc, sizeof(uint8_t));
      ptr++;
      memcpy((void*)(_pkt_bin), (void*)&pkt.pkt_len, sizeof(uint16_t));
      ptr++;
      ptr++;
      memcpy((void*)(_pkt_bin), (void*)&pkt.op_code, sizeof(uint8_t));
      ptr++;
      
      for (int i = 0 ; i < pkt.payload_len ; i++)
      {
        memcpy((void*)(_pkt_bin + 4 + i), (void*)(pkt.payload + i), sizeof(uint8_t));
        ptr++;
      }

      update_checksum();

      memcpy((void*)(_pkt_bin + 4 + pkt.payload_len), (void*)&pkt.chk, sizeof(uint8_t));
    }

    BM64_uart_pkt_t pkt;
    uint8_t* _pkt_bin;
    uint16_t _pkt_len;

    void update_checksum()
    {
      pkt.chk = 0;
      for (int i=1 ; i<(pkt.payload_len+4) ; i++)
      {
        pkt.chk += *(_pkt_bin+i);
      }
      pkt.chk = ((pkt.payload_len + 3) << 2) - pkt.chk;
    }
};
