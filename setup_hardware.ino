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

void pin_setup(void){
  // MFA Control buttons
  // Mode switch button
  pinMode(PIN_MODE, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_MODE), PIN_mode_changed, CHANGE);
  PIN_mode_state = digitalRead(PIN_MODE);
  // Reset switch button
  pinMode(PIN_RESET, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_RESET), PIN_reset_changed, CHANGE);
  PIN_reset_state = digitalRead(PIN_RESET);
  // Memory switch button
  pinMode(PIN_MEMORY, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_MEMORY), PIN_memory_changed, CHANGE);
  PIN_memory_state = digitalRead(PIN_MEMORY);

  // Analog and digital PIN
  // Voltage PIN ; IN
  pinMode(PIN_Volt, INPUT_PULLUP);

  // Stay On PIN ; OUT
  pinMode(PIN_STAY_ON, OUTPUT);

  // Door right PIN ; IN
  pinMode(PIN_R_DOOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_R_DOOR), PIN_R_Door_changed, CHANGE);
  door_r = digitalRead(PIN_R_DOOR);

  // Door Sliding PIN ; IN
  pinMode(PIN_S_DOOR, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_S_DOOR), PIN_S_Door_changed, CHANGE);
  door_s = digitalRead(PIN_S_DOOR);

  // Motor Cap PIN ; IN
  pinMode(PIN_MOTOR_CAP, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_CAP), PIN_motor_cap_changed, CHANGE);
  motor_cap = digitalRead(PIN_MOTOR_CAP);

  // Trunk PIN ; IN
  pinMode(PIN_TRUNK, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_TRUNK), PIN_Trunk_changed, CHANGE);
  trunk = digitalRead(PIN_TRUNK);

  // Wiper Water Warning PIN ; IN
  // pinMode(PIN_WASHER_FLUID, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(PIN_WASHER_FLUID), PIN_washer_fluid_changed, CHANGE);
  // washer_fluid = digitalRead(PIN_WASHER_FLUID);

  // Brake Pad PIN ; IN
  pinMode(PIN_BRAKEPADS, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_BRAKEPADS), PIN_brakepads_changed, CHANGE);
  brakepads = digitalRead(PIN_BRAKEPADS);

  // Oil Presure PIN ; IN
  pinMode(PIN_OIL_PRESURE, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_OIL_PRESURE), PIN_Oil_Presure_changed, CHANGE);
  oil_presure = digitalRead(PIN_OIL_PRESURE);

  // Oil Level PIN ; IN
  pinMode(PIN_OIL_LEVEL, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_OIL_LEVEL), PIN_Oil_Level_changed, CHANGE);
  oil_level = digitalRead(PIN_OIL_LEVEL);

}

void PIN_mode_changed(void){
  PIN_mode_state = digitalRead(PIN_MODE);
}

void PIN_reset_changed(void){
  PIN_reset_state = digitalRead(PIN_RESET);
}

void PIN_memory_changed(void){
  PIN_memory_state = digitalRead(PIN_MEMORY);
}

void PIN_R_Door_changed(void){
  door_r = digitalRead(PIN_R_DOOR);
  check_led = true;
}

void PIN_S_Door_changed(void){
  door_s = digitalRead(PIN_S_DOOR);
  check_led = true;
}

void PIN_motor_cap_changed(void){
  motor_cap = digitalRead(PIN_MOTOR_CAP);
  check_led = true;
}

void PIN_Trunk_changed(void){
  trunk = digitalRead(PIN_TRUNK);
  check_led = true;
}

// void PIN_washer_fluid_changed(void){
//   washer_fluid = digitalRead(PIN_WASHER_FLUID);
// //  check_led = true;
// }

void PIN_brakepads_changed(void){
  brakepads = digitalRead(PIN_BRAKEPADS);
  check_led = true;
}

void PIN_Oil_Presure_changed(void){
  oil_presure = digitalRead(PIN_OIL_PRESURE);
  check_led = true;
}

void PIN_Oil_Level_changed(void){
  oil_level = digitalRead(PIN_OIL_LEVEL);
  check_led = true;
}


// void pin_init(void){
//   // Init the button state, as otherwise it will be set only after first press of the button
//   PIN_mode_state = digitalRead(PIN_MODE);
//   PIN_reset_state = digitalRead(PIN_RESET);
//   PIN_memory_state = digitalRead(PIN_MEMORY);
// }
