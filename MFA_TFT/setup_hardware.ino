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

  // configure MCP pin that will read INTA/B state
  pinMode(INT_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), ISR_INT_PIN, CHANGE);

  // pinMode(MCP_RESET, OUTPUT);
  // digitalWrite(MCP_RESET, HIGH);

  // OPTIONAL - call this to override defaults
  // mirror INTA/B so only one wire required
  // active drive so INTA/B will not be floating
  // INTA/B will be signaled with a LOW
  mcp.setupInterrupts(true, false, LOW);

  // configure button pin for input with pull up
  for (byte i = 0; i < 16; i++){
    mcp.pinMode(i, INPUT_PULLUP);
    // enable interrupt on button_pin
    mcp.setupInterruptPin(i, CHANGE);
  }


  // Analog and digital PIN
  // Voltage PIN ; IN
  //pinMode(PIN_Volt, INPUT); Not needed for analog read


  // // Door right PIN ; IN
  // pinMode(PIN_R_DOOR, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_R_DOOR), PIN_R_Door_changed, CHANGE);
  // door_r = digitalRead(PIN_R_DOOR);

  // // Door Sliding PIN ; IN
  // pinMode(PIN_S_DOOR, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_S_DOOR), PIN_S_Door_changed, CHANGE);
  // door_s = digitalRead(PIN_S_DOOR);

  // // Motor Cap PIN ; IN
  // pinMode(PIN_MOTOR_CAP, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_CAP), PIN_motor_cap_changed, CHANGE);
  // motor_cap = digitalRead(PIN_MOTOR_CAP);

  // // Trunk PIN ; IN
  // pinMode(PIN_TRUNK, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_TRUNK), PIN_Trunk_changed, CHANGE);
  // trunk = digitalRead(PIN_TRUNK);

  // //Wiper Water Warning PIN ; IN
  // pinMode(PIN_WASHER_FLUID, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_WASHER_FLUID), PIN_washer_fluid_changed, CHANGE);
  // washer_fluid = digitalRead(PIN_WASHER_FLUID);

  // // Brake Pad PIN ; IN
  // pinMode(PIN_BRAKEPADS, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_BRAKEPADS), PIN_brakepads_changed, CHANGE);
  // brakepads = digitalRead(PIN_BRAKEPADS);

  // // Brake System PIN ; IN
  // pinMode(PIN_BRAKESYSTEM, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_BRAKESYSTEM), PIN_brakesystem_changed, CHANGE);
  // brakesystem = digitalRead(PIN_BRAKESYSTEM);

  // // Oil Presure PIN ; IN
  // pinMode(PIN_OIL_PRESURE, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_OIL_PRESURE), PIN_Oil_Presure_changed, CHANGE);
  // oil_presure = digitalRead(PIN_OIL_PRESURE);

  // // Oil Level PIN ; IN
  // pinMode(PIN_OIL_LEVEL, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_OIL_LEVEL), PIN_Oil_Level_changed, CHANGE);
  // oil_level = digitalRead(PIN_OIL_LEVEL);

// ToDo:
  //pinMode(PIN_OIL_PWM, INPUT);

}

// void pin_init(void){
//   // Init the button state, as otherwise it will be set only after first press of the button
//   PIN_mode_state = digitalRead(PIN_MODE);
//   PIN_reset_state = digitalRead(PIN_RESET);
//   PIN_memory_state = digitalRead(PIN_MEMORY);
// }
