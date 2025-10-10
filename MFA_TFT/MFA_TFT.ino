/*
  This is a Volkswagen MFA adapted to use CAN Bus from T4
  Based on an ESP32 and RPI TFT Color Display
  Data will be collected by a WCMCU-230 CAN Bus Modul

  Please note, that the RPI display has 480 x 320 Pixel, but 
  the visible area will only be: (not modifying the plastik parts of the car) 
  min X_Pos = 10 and max X_Pos = 310
  min Y_Pos = 40 and max Y_Pos = 457
  resuling in a usable width = 300 and height = 417
  if we use a security distance = 4 to avoid mounting differences
  min X_Pos = 14 and max X_Pos = 306
  min Y_Pos = 44 and max Y_Pos = 453
  resuling in a usable width = 292 and height = 409

  Copyright (c) 2024, nieson@web.de
  All rights reserved.
*/

// include Arduino libraries
#include "TFT_eSPI.h"
#include "Free_Fonts.h"
//#include "FreeSansBold24pt7b.h"
#include <ESP32-TWAI-CAN.hpp>
#include <Preferences.h>
#include <Wire.h>
#include <Adafruit_MCP23X17.h>

// include our own files
// Pin definition and settings see pin_setup.h
#include "pin_setup.h"
#include "variables.h"

portMUX_TYPE dataMux = portMUX_INITIALIZER_UNLOCKED;

// include images and symbols
#include "symbols.h"
//#include "logo.h"
#include "T4_Image.h"

// make TFT instance
TFT_eSPI tft = TFT_eSPI();

// make Sprite for the dial (avoid flicker effects)
TFT_eSprite dial = TFT_eSprite(&tft);
TFT_eSprite scale = TFT_eSprite(&tft);
TFT_eSprite needle = TFT_eSprite(&tft);
// make Sprite for the values (avoid flicker effects)
TFT_eSprite box = TFT_eSprite(&tft);
TFT_eSprite left_bar = TFT_eSprite(&tft);
TFT_eSprite right_bar = TFT_eSprite(&tft);

// set up can system
twai_filter_config_t filter;
CanFrame rxFrame;

// need preferences to store values in NVS (non volatile storage)
Preferences preferences;

// define MCP instance
Adafruit_MCP23X17 mcp;

// Use second kernel to evaluate can messages
TaskHandle_t EvaluateCAN;

// function defination to handle also default values
void drawUnits(int Y_Pos, String upper_line, String lower_line = "");

// -- Helper functions for critical section safe access --
void get_data_snapshot(int* page, int* mode, float* last25km) {
  portENTER_CRITICAL(&dataMux);
  *page = Data.page;
  *mode = Data.mode;
  *last25km = C_last_25_km;
  portEXIT_CRITICAL(&dataMux);
}

bool fetch_and_clear_check_led() {
  bool flag;
  portENTER_CRITICAL(&dataMux);
  flag = check_led;
  check_led = false;
  portEXIT_CRITICAL(&dataMux);
  return flag;
}

void safe_update_mode() {
  portENTER_CRITICAL(&dataMux);
  Data.mode += 1;
  if (Data.mode > 2) {
    Data.mode = 0;
  }
  portEXIT_CRITICAL(&dataMux);
}

void safe_update_page() {
  portENTER_CRITICAL(&dataMux);
  Data.page += 1;
  if (Data.page > 4) {
    Data.page = 0;
  }
  portEXIT_CRITICAL(&dataMux);
}

// ------------------------------------------------------

void setup(void) {

  Serial.begin(115200);

  // start the TFT Display and set orientation
  setup_TFT();

  // Set up can system
  setup_CAN();

  // setup the IO expansion board
  setup_MCP();

  // PIN setup
  pin_setup();

  // create some sprites to avoid to much screen drawing
  create_sprites();

  // load the Data stored in memory
  load_Data();

  // DEBUG:
  portENTER_CRITICAL(&dataMux);
  Data.page = 0;
  Data.mode = START;
  portEXIT_CRITICAL(&dataMux);

  temp_page = Data.page;

  // Create Task on Core 0 to read CAN Messages and not delaying due to TFT Drawing functions
  xTaskCreatePinnedToCore(CAN_Loop, "CAN_Loop", 1000, NULL, 0, &EvaluateCAN, 0);

  Serial.println("Looping...");
  mcp.clearInterrupts();  // clear
}

// Main loop running on Core 1 handles all drawing of TFT and IO Stuff
void loop(void) {
  // Get a safe snapshot of important shared data
  int page, mode;
  float last25km;
  get_data_snapshot(&page, &mode, &last25km);

  // we are not able to do any hardware stuff on the second task, as it will lead to crashes
  // That's the reason we are doing it in main loop
  if (save == true) {
    save_Data();
    save = false;
  }

  if (reset == true) {
    reset_Data(mode);
    //avoid page change
    reset = false;
  }

  // initial drawing the screen
  if(start == false) {
    draw_InitPage();
    delay(2000);

    DrawSelected(page);
    start = true;
    Serial.println("Start");

    // We do an initial check for the mccp state, as we may have unatendet IO's
    for (byte i = 0; i < 16; i++) {
      check_IO(i, mcp.digitalRead(i));
    }
  }

  // update time every second
  if (millis() - lastMillis > 1000) {
    update_time();
    lastMillis = millis();
  }

  // DEBUG : Check LED behavior
  motor_on = true;
  // petrol = true;
  // batterie = true;
  // brakepads = true;
  // brakesystem = true;
  // washer_fluid = true;
  // oil_presure = true;
  // door = true;
  // coolant = true;
  //warnings = 511;

  // let the symbol blink, so we do not need a special place for the LED
  // can not be done in check LED, as it will lead to a blinking dial in this case line is 316
  // and should not been done if shutdown timer is running
  if (shutdown_timer == 0) {
    bool is_petrol;
    portENTER_CRITICAL(&dataMux);
    is_petrol = petrol;
    portEXIT_CRITICAL(&dataMux);

    if(is_petrol == true){
      int ts;
      portENTER_CRITICAL(&dataMux);
      ts = Data.time_start;
      portEXIT_CRITICAL(&dataMux);

      if(ts % 2 != 0){
        temp_color = TFT_ORANGE;
      } else {
        temp_color = TEXT_COLOR;
      }
      tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, temp_color);
    }
    else if ((is_petrol == false) && (temp_color == TFT_ORANGE)){
      tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, TEXT_COLOR);
    }
  }

  update_volt();

  // Use critical section for check_led read/clear
  if (fetch_and_clear_check_led()) {
    check_LED();
  }

  // to avoid update values for door warning in full page mode
  int temp_pg;
  portENTER_CRITICAL(&dataMux);
  temp_pg = temp_page;
  portEXIT_CRITICAL(&dataMux);
  if (page == temp_pg) {
    update_values();
  }

  // stay_on == true after we had one time ignition
  // shutdown_timer will be set switching off ignition 
  if ((shutdown_timer != 0) && (millis() > shutdown_timer + 3600000)) {
    shutdown_timer = 0;
    digitalWrite(PIN_STAY_ON, 0);
  }

  if((Mode_Button_pressed != 0) && (millis() > Mode_Button_pressed + 200)) {
    safe_update_mode();
    Mode_Button_pressed = 0;
  }

  if((Reset_Button_pressed != 0) && (millis() > Reset_Button_pressed + 3000)) {
    reset = true;
    Reset_Button_pressed = 0;
  }

  if (!PIN_INT_state) {
    volatile int PIN = mcp.getLastInterruptPin();
    volatile int mcp_state = mcp.getCapturedInterrupt();

    check_IO(PIN, bitRead(mcp.getCapturedInterrupt(), PIN));
    Serial.println("Checked Interupted PIN");
  }
}

void switch_page(void) {
  safe_update_page();
  int page_snapshot;
  portENTER_CRITICAL(&dataMux);
  page_snapshot = Data.page;
  portEXIT_CRITICAL(&dataMux);

  temp_page = page_snapshot;
  DrawSelected(page_snapshot);
  Serial.println("Button");
  check_LED();
}

// This loop runs on Core 0, while the main loop runs on Core 1
void CAN_Loop (void *parameter) {
  // create infinite loop
  for(;;){
    // We set custom timeout to 0, default is 1000
    if(ESP32Can.readFrame(rxFrame, 0)) {
      evaluate_CAN_messages();
    }
  }
}

void ISR_INT_PIN(void) {
  PIN_INT_state = digitalRead(INT_PIN);
}