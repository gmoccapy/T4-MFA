void setup_TFT(void){
  // start the TFT Display and set orientation
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(BACK_COLOR);
  tft.setTextColor(TEXT_COLOR, BACK_COLOR);
}

void setup_CAN(void){
  // Set up can system
  // set up filters, need to be done in a better way, as this will accept all ID'S
// ToDo: calculate filter corectly to avoid unneeded trafic
  filter.acceptance_code = 0x280 << 5;
  filter.acceptance_mask = 0x7A8 << 5;
  filter.single_filter = false;
  ESP32Can.begin(ESP32Can.convertSpeed(500), CAN_TX, CAN_RX, 0, 20, &filter);
}

void setup_MCP(void){
  Wire.begin(26, 27);

  if (!mcp.begin_I2C(0x20)) {
    tft.fillScreen(TFT_BLACK);
    tft.drawString("Error MCP Start.", 50, 200, FONT4);
  }

}
void pin_setup(void){

  // Stay On PIN ; OUT
  pinMode(PIN_STAY_ON, OUTPUT);

  // OIL LED comes directly from GPIO 25:
  pinMode(PIN_OIL_LED, INPUT);

// TODO: PIN_OIL_PWN as Input from PIN31 from ESP32 coresponds to GPIO 19 
//pinMode(PIN_OIL_PWM, INPUT);

  // configure MCP pin that will read INTA/B state
  pinMode(INT_PIN, INPUT);

  // OPTIONAL - call this to override defaults
  // mirror INTA/B so only one wire required
  // active drive so INTA/B will not be floating
  // INTA/B will be signaled with a LOW
  mcp.setupInterrupts(true, false, LOW);

  // configure button pin for input with pull up
  for (byte i = 0; i < 16; i++){
    mcp.pinMode(i, INPUT);
    // enable interrupt on button_pin
    mcp.setupInterruptPin(i, CHANGE);
  }
  attachInterrupt(digitalPinToInterrupt(INT_PIN), ISR_INT_PIN, CHANGE);

}

