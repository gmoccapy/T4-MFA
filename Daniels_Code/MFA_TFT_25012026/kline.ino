///////////
///////////
// komplette kline logik ungeprüft - bzw- funktioniert so nicht
///////////
///////////

#include <Arduino.h>

  uint8_t calc_checksum(const uint8_t* data, uint8_t len) {
  uint8_t sum = 0;
  for (uint8_t i = 3; i < len; i++) {    // ab Byte 3
    sum += data[i];
  }
  return sum;
}

void handle_KLine() {
  unsigned long now = millis();

  // Init
  if (!kline_active && now - kline_last_init > 3000) {
    KLine.write(0x33);
    KLine.flush();
    kline_active = true;
    kline_last_request = now;
 //   Serial.println("K-Line 5-Baud-Init gesendet");
  }

  if (kline_active && now - kline_last_request >= 700) {
    if (kline_state == KLINE_IDLE) {
      request_oil_level();
      kline_state = KLINE_WAIT_OIL;
    }
    else if (kline_state == KLINE_WAIT_OIL && now - kline_last_request >= 100) {
      request_boost_pressure();
      kline_state = KLINE_IDLE;
    }
    kline_last_request = now;
  }

  kline_parse_responses();

//  static unsigned long last_warning_update = 0;
//  if (millis() - last_warning_update >= 1000) {   
//    update_oil_level_warning();                   
//    check_led = true;         
//    last_warning_update = millis();
//  }
}



void request_oil_level() {
  uint8_t cmd[] = {0x21, 0x81, 0xE2};
  send_kline_cmd(cmd, sizeof(cmd));
}

void request_boost_pressure() {
  uint8_t cmd[] = {0x21, 0x83, 0xE4};
  send_kline_cmd(cmd, sizeof(cmd));
}

void send_kline_cmd(const uint8_t* data, uint8_t len) {
  uint8_t chk = 0;
  KLine.write(0x68); KLine.write(0x6A); KLine.write(0xF1);
  for (uint8_t i = 0; i < len; i++) {
    KLine.write(data[i]);
    chk += data[i];
  }
  KLine.write(chk);
}

void kline_parse_responses() {
  while (KLine.available()) {
    uint8_t b = KLine.read();
    kline_last_byte_time = millis();   // Timeout-Reset

    static uint8_t buf[32];
    static uint8_t idx = 0;

    if (idx == 0 && b != 0x68) continue;
    buf[idx++] = b;

    if (idx >= 7 && idx == buf[0] + 5) {
      if (buf[idx-1] == calc_checksum(buf, idx-1)) {
        if (buf[3] == 0x61 && buf[4] == 0x81) {
          oil_level_mm = (buf[5] << 8 | buf[6]) * 0.1f;
        }
        if (buf[3] == 0x61 && buf[4] == 0x83) {
          boost_mbar = (buf[5] << 8 | buf[6]);
        }
      }
      idx = 0;
    }
    if (idx >= sizeof(buf)) idx = 0;
  }

  //  Timeout  Reset bei >1 Sekunde still
  if (kline_active && millis() - kline_last_byte_time > 1000) {
    // optional: Neu-Init erzwingen
    // kline_active = false; kline_last_init = millis();
  }
}

void update_oil_level_warning() {
  static float last_valid_oil = 99.0f;
  if (oil_level_mm >= 0.0f && oil_level_mm < 50.0f) {   
    last_valid_oil = oil_level_mm;

    if (oil_level_mm < 12.0f) {        
      oil_level = false;               
    }
    else if (oil_level_mm < 15.0f) {   // 12–15mm 
      oil_level = false;               
    }
    else {
      oil_level = true;               
    }
  }
}