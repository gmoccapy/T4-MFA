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
  Data.page = 0;
  Data.mode = START;

  temp_page = Data.page;

  // Create Task on Core 0 to read CAN Messages and not delaying due to TFT Drawing functions
  xTaskCreatePinnedToCore(CAN_Loop, "CAN_Loop", 1000, NULL, 0, &EvaluateCAN, 0);

  Serial.println("Looping...");
  mcp.clearInterrupts();  // clear
}

// Main loop running on Core 1 handles all drawing of TFT and IO Stuff
void loop(void) {

// DEBUG
  // Serial.print(time_C_period);
  // Serial.print("\t");
  // Serial.print(C_actual * 100.0 / velocity_actual);
  // Serial.print("\t");
  // Serial.print(C_motor_value);
  // Serial.print("\t");
  // Serial.println(velocity_actual);


  // we are not able to do any hardware stuff on the second task, as it will lead to crashes
  // That's the reason we are doing it in main loop
  if (save == true){
    save_Data();
    save = false;
  }

  if (reset != 0){
    reset_Data(reset);
    //avoid page change
    reset = 0;
  }

  // initial drawing the screen
  if(start == false){
    // draw the Starting image
    draw_InitPage();
    delay(2000);

    DrawSelected(Data.page);
    start = true;
    Serial.println("Start");
    
    // We do an initial check for the mccp state, as we may have unatendet IO's
    for (byte i = 0; i < 16; i++){
      check_IO(i, mcp.digitalRead(i));
    }

  }

  // update time every second
  if (millis() - lastMillis > 1000){
    update_time();
    lastMillis = millis();
  }

  // DEBUG : Check Battreie LED behavior
  //motor_on = true;

  update_volt();

  if (check_led == true){
     check_LED();
     check_led = false;
  }

  // to avoid update values for door warning in full page mode
  if (Data.page == temp_page){
    update_values();
  }

  // stay_on == true after we had one time ignition
  // shutdown_timer will be set switching off ignition 
  if ((shutdown_timer != 0) && (millis() > shutdown_timer + 3600000)){
    shutdown_timer = 0;
    digitalWrite(PIN_STAY_ON, 0);
  }

  if((Mode_Button_pressed != 0) && (millis() > Mode_Button_pressed + 3000)){
    reset = Data.mode;
    Mode_Button_pressed = 0;
    Page_Switch_Done = true;                // If a reset has been requested, we do not want Page Change
  }

  if (!PIN_INT_state){
    volatile int PIN = mcp.getLastInterruptPin();
    volatile int mcp_state = mcp.getCapturedInterrupt();

    Serial.print("Interupted PIN = ");
    Serial.println(PIN);
    Serial.print("Pin states at time of interrupt: ");
    //Serial.println(mcp.getCapturedInterrupt(), 2);
    
    Serial.println(bitRead(mcp.getCapturedInterrupt(), PIN));

    check_IO(PIN, bitRead(mcp.getCapturedInterrupt(), PIN));

    Serial.println("  Checked");
  }


}

void switch_page(void){
  Data.page += 1;
  if (Data.page > 4){
    Data.page = 0;
  }
  temp_page = Data.page;
  Page_Switch_Done = true;
  DrawSelected(Data.page);
  Serial.println("Button");
  check_LED();
}

// This loop runs on Core 0, while the main loop runs on Core 1
void CAN_Loop (void *parameter){
  // create infinite loop
  for(;;){
    // We set custom timeout to 0, default is 1000
    if(ESP32Can.readFrame(rxFrame, 0)) {
      evaluate_CAN_messages();
    }
  }
}

void ISR_INT_PIN(void){
  PIN_INT_state = digitalRead(INT_PIN);
}

