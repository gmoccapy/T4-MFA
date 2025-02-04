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

  // Setup the hardware
  // PIN setup
  pin_setup();

  // start the TFT Display and set orientation
  setup_TFT();

  // Set up can system
  setup_CAN();

  // need to initialize some PIN, set to default
  //pin_init();

  // create some sprites to avoid to much screen drawing
  create_sprites();

  // load the Data stored in memory
  load_Data();
  temp_page = Data.page;

// DEBUG:
  Data.page = 0;
  Data.mode = START;

  // Create Task on Core 0 to read CAN Messages and not delaying due to TFT Drawing functions
//  xTaskCreatePinnedToCore(CAN_Loop, "CAN_Loop", 1000, NULL, 0, &EvaluateCAN, 0);

}

// Main loop running on Core 1 handles all drawing of TFT and IO Stuff
void loop(void) {

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
    //check_LED();
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
    //check_LED();
  }

  // update time every second
  if (millis() - lastMillis > 1000){
    update_time();
    lastMillis = millis();
  }

  // if (check_led == true){
  //   check_LED();
  // }

  // to avoid update values for door warning in full page mode
  if (Data.page == temp_page){
    update_values();
  }

  update_volt();

  // Serial.print("door = ");
  // Serial.print(door);
  // Serial.print("\t");
  // Serial.print("light = ");
  // Serial.print(light);
  // Serial.print("\t");
  // Serial.print("petrol = ");
  // Serial.print(petrol);
  // Serial.print("\t");
  // Serial.print("presure = ");
  // Serial.print(oil_presure);
  // Serial.print("\t");
  // Serial.print("level = ");
  // Serial.print(oil_level);
  // Serial.print("\t");
  // Serial.print("counter = ");
  // Serial.print(counter);
  // Serial.println("\t");
  

  // stay_on == true after we had one time ignition
  // shutdown_timer will be set switching off ignition 
  if ((shutdown_timer != 0) && (millis() > shutdown_timer + 3600000)){
    shutdown_timer = 0;
    digitalWrite(PIN_STAY_ON, 0);
  }
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
