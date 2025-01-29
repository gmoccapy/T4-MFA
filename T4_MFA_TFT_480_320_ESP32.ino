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

#include "pin_setup.h"
#include "TFT_eSPI.h"
#include "Free_Fonts.h"
#include "variables.h"
#include "symbols.h"
//#include "logo.h"
#include "T4_Image.h"
#include <ESP32-TWAI-CAN.hpp>
#include <Preferences.h>

//#include "FreeSansBold24pt7b.h"

// Pin definition and settings see pin_setup.h

// make TFT instance
TFT_eSPI tft = TFT_eSPI();

// make Sprite for the dial (avoid flicker effects)
TFT_eSprite dial = TFT_eSprite(&tft);
TFT_eSprite scale = TFT_eSprite(&tft);
TFT_eSprite needle = TFT_eSprite(&tft);
// make Sprite for the values (avoid flicker effects)
TFT_eSprite box = TFT_eSprite(&tft);


// set up can system
twai_filter_config_t filter;
CanFrame rxFrame;

// need preferences to store values in NVS (non volatile storage)
Preferences preferences;

// Use second kernel to evaluate can messages
TaskHandle_t EvaluateCAN;

// function defination to handle also default values
void drawUnits(int Y_Pos, String upper_line, String lower_line = "");

void setup(void) {

  Serial.begin(115200);

  // MFA Control buttons
  // Mode switch button settings
  pinMode(PIN_Mode, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_Mode), PIN_mode_changed, CHANGE);
  pinMode(PIN_Reset, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_Reset), PIN_reset_changed, CHANGE);
  pinMode(PIN_Memory, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_Memory), PIN_memory_changed, CHANGE);

  // Analog and digital PIN
  // Voltage PIN ; IN
  pinMode(PIN_Volt, INPUT_PULLUP);
  // Stay On PIN ; OUT
  pinMode(PIN_STAY_ON, OUTPUT);
  // Trunk PIN ; IN
  pinMode(PIN_TRUNK, INPUT_PULLUP);
  // Oil Presure PIN ; IN
  pinMode(PIN_OIL_PRESURE, INPUT_PULLUP);
  // Door right PIN ; IN
  pinMode(PIN_R_DOOR, INPUT_PULLUP);
  // Door Sliding PIN ; IN
  pinMode(PIN_S_DOOR, INPUT_PULLUP);
  // Wiper Water Warning PIN ; IN
  pinMode(PIN_WIPER_WATER, INPUT_PULLUP);
  // Brake Pad PIN ; IN
  pinMode(PIN_BRAKE_PAD, INPUT_PULLUP);
  // Motor Cap PIN ; IN
  pinMode(PIN_MOTOR_CAP, INPUT_PULLUP);
  // Oil Level PIN ; IN
  pinMode(PIN_OIL_LEVEL, INPUT_PULLUP);

  // start the TFT Display and set orientation
  tft.begin();
  tft.setRotation(0);
  tft.fillScreen(BACK_COLOR);
  tft.setTextColor(TEXT_COLOR, BACK_COLOR);

  create_sprites();

  // Set up can system
  // set up filters, need to be done in a better way, as this will accept all ID'S
// ToDo: calculate filter corectly to avoid unneeded trafic
  filter.acceptance_code = 0x280 << 5;
  filter.acceptance_mask = 0x7A8 << 5;
  filter.single_filter = false;
  ESP32Can.begin(ESP32Can.convertSpeed(500), CAN_TX, CAN_RX, 0, 20, &filter);

  // Init the button state, as otherwise it will be set only after first press of the button
  PIN_mode_state = digitalRead(PIN_Mode);
  PIN_reset_state = digitalRead(PIN_Reset);
  PIN_memory_state = digitalRead(PIN_Memory);

  // load the stored Data in memory
  load_Data();
  temp_page = Data.page;

  Data.page = 4;

  // Create Task on Core 0 to read CAN Messages and not delaying due to TFT Drawing functions
  xTaskCreatePinnedToCore(CAN_Loop, "CAN_Loop", 1000, NULL, 0, &EvaluateCAN, 0);

}

// Main loop running on Core 1 handles all drawing of TFT and IO Stuff
void loop(void) {

// display size of CAN Queue
//  dtostrf(ESP32Can.inRxQueue(), 4, 0, TFT_String);
//  draw_small_value_box(320, 460, 50, 30, TFT_String);

  
  // we are not able to do any hardware stuff on the second task, as it will lead to crashes
  // That's the reason we are doing it in main loop
  if (save == true){
    save_Data();
    save = false;
  }

  if (reset != 0){
    reset_Data(reset);
    //avoid page change
    PIN_mode_previous_state = PIN_mode_state;
    reset = 0;
  }

  if ((PIN_mode_state != PIN_mode_previous_state) && (millis() - previousPressedMode > debounce)){
    if (PIN_mode_state == true){
      if(reset == 0){
        Data.page += 1;
        if (Data.page > 4){
          Data.page = 0;
        }
        temp_page = Data.page;
        
        //print_Data();
      }
//ToDo : Check if this else is realy needed
      else{
        reset = 0;
      }
    }
    PIN_mode_previous_state = PIN_mode_state;
    previousPressedMode = millis();
    DrawSelected(Data.page);
    //print_Data();

    // if (Data.page == 3){
    //   tft.drawXBitmap(Icon_Pos_Door[0]  , Icon_Pos_Door[1],               sym_door,          50, 50, TFT_ORANGE);
    //   //tft.drawXBitmap(Icon_Pos_Light[0] , Icon_Pos_Light[1],              sym_light,         50, 50, TFT_GREEN);
    //   tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1],             sym_petrol,        50, 50, TFT_ORANGE);
    //   tft.drawXBitmap(Icon_Pos_Oil[0]  , Icon_Pos_Oil[1],                 sym_oil,           50, 50, TFT_RED);
    //   tft.drawXBitmap(Icon_Pos_WasherFluid[0] , Icon_Pos_WasherFluid[1],  sym_washer_fluid,  50, 50, TFT_ORANGE);
    //   tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1],           sym_coolant,       50, 50, TFT_RED);
    //   tft.drawXBitmap(Icon_Pos_BrakePads[0] , Icon_Pos_BrakePads[1],      sym_brakepads,     50, 50, TFT_ORANGE);
    // }

  }

  // User reset
  if(PIN_mode_state == false){
    if(previousPressedMode + 3000 < millis()){
      // We do not want to change page, but this will hapen releasing the button, so we set reset
      reset = Data.mode;
    }
  }

  // initial drawing the screen
  if(start == false){
    // draw the Starting image
    draw_InitPage();
    delay(2000);

    DrawSelected(Data.page);
    start = true;
  }

  // update time every second
  if (millis() - lastMillis > 1000){
    update_time();
    lastMillis = millis();
  }

  if (check_led == true){
    check_LED();
    check_led = false;
  }

  // to avoid update values for door warning in full page mode
  if (Data.page == temp_page){
    update_values();
  }

  update_volt();

  Serial.println(PIN_mode_state);

  // temp_oil += 1;
  // if (temp_oil > 160){
  //   temp_oil = 0;
  // }
  // // draw oil temp scale
  // String(F("Oil")).toCharArray(TFT_String, 4);
  // draw_bar(TFT_String, 15, 295, temp_oil);

  //draw_dial(75, 108, temp_oil, 0, temp_oil, 160.0, F("Öl"), F("Temperatur"));



  // stay_on == true after we had one time ignition
  // shutdown_timer will be set switching off ignition 
  if ((shutdown_timer != 0) && (millis() > shutdown_timer + 3600000)){
    shutdown_timer = 0;
    digitalWrite(PIN_STAY_ON, 0);
  }
}

void PIN_mode_changed(void){
  PIN_mode_state = digitalRead(PIN_Mode);
}

void PIN_reset_changed(void){
  PIN_reset_state = digitalRead(PIN_Reset);
}

void PIN_memory_changed(void){
  PIN_memory_state = digitalRead(PIN_Memory);
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
