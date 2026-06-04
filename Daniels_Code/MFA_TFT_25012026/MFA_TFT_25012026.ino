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

  Copyright (c) 2024, nieson@web.de and Daniel Braun
  All rights reserved.
*/



//#define with_DIAL   // wenn eingeschaltet,SYM_NA_VISIBLE ausschalten!
#define SYM_NA_VISIBLE // sollen die "ausgeschalteten" LED sichtbar sein? -> Macht nur Sinn, wenn DIAL ausgeschaltet ist.

// #define DEBUG_TFT
// #define DEBUG_CAN
// #define DEBUG_MCP

// include Arduino libraries
#include <TinyGPS++.h>
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
#include "logo.h"
//#include "T4_Image.h"

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
TFT_eSprite compass = TFT_eSprite(&tft);

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

  // load the Data stored in memory
  load_Data();

  // setup GPS functions
  setup_GPS();

  // setup Kline Communication
  //setup_KLine();

  // create some sprites to avoid to much screen drawing
  create_sprites();
 
  // DEBUG:
  Data.page = 0;
  Data.mode = START;


  temp_page = Data.page;
  
  // Create Task on Core 0 to read CAN Messages and not delaying due to TFT Drawing functions
  xTaskCreatePinnedToCore(CAN_Loop, "CAN_Loop", 1000, NULL, 0, &EvaluateCAN, 0);

 // Serial.println("Looping...");
  mcp.clearInterrupts();  // clear

// Initialisiere currentMemory basierend auf aktuellem Memory_Button_state (lese MCP früh)
currentMemory = Memory_Button_state ? 0 : 1;
previousMemoryState = Memory_Button_state;
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

  // initial drawing the screen
  if(start == false){
  // draw the Starting image
    draw_InitPage();
    delay(2000);

    DrawSelected(Data.page);
    start = true;
    //Serial.println("Start"); 
    check_led = true;
    
    // We do an initial check for the mccp state, as we may have unatendet IO's
    check_IO();
  }

  // update time every second
  //if (millis() - lastMillis > 1000){
  //  update_time();
  //  lastMillis = millis();
 // }

// DEBUG : Check LED behavior
  // motor_on = true;
  // petrol = true;
  // light = true;
  // battery = true;
  // brakepads = true;
  // brakesystem = true;
  // washer_fluid = true;
  // oil_pressure = true;
  // door = true;
  // coolant = true;
  // warnings = 511;

  // let the symbol blink, so we do not need a special place for the LED
  // can not be done in check LED, as it will lead to a blinking dial in this case line is 316
  // and should not been done if shutdown timer is running
  
   if (shutdown_timer == 0){
   // if(Data.time_start % 2 != 0){
   //   temp_color = TFT_ORANGE;
   // }
   // else{
   // temp_color = TEXT_COLOR;
  //  }

  if((currentMemory == 0) && (petrol == true) && (Data.page == 0)){
   tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, TFT_ORANGE);  // temp_color verwenden bei gewünschtem blinken
   }
   if ((currentMemory == 0) && (petrol == false) && (Data.page == 0)){ //&& (temp_color == TFT_ORANGE) ){
   tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, SYM_NA_COLOR);
   }

// wird in draw_screen.ino berücksichtigt    prüfen, ob draw_screen oft genug aufgerufen wird
 //Draw Snowflake only if Temp under 4 degrees

}

  update_volt();

  if (check_led == true){
     check_LED();
     check_led = false;
  }

  // to avoid update values for warnings in full page mode
  if (Data.page == temp_page){
    update_values();
  }

  // stay_on == true after we had one time ignition
  // shutdown_timer will be set switching off ignition 
  if ((shutdown_timer != 0) && (millis() > shutdown_timer + shutdown_time)){  
    shutdown_timer = 0;
    digitalWrite(PIN_STAY_ON, 0);
  }

//Mode Button nach IO_handlings verschoben

  if((Reset_Button_pressed != 0) && (millis() > Reset_Button_pressed + 3000)){
    if (Data.mode == PERIOD){
    pending_reset = PERIOD;
    Reset_Button_pressed = 0;
    }
   // if (Data.mode ==REFUEL){
   //   pending_reset = REFUEL;
   //   Reset_Button_pressed = 0;
   // }
  }

uint8_t current_reset = pending_reset;
if (current_reset != NOTHING) {
 reset_Data(current_reset);
  pending_reset = NOTHING;  // reset zurücksetzen
  save = true;
}

  if (!PIN_INT_state){
    check_IO();
    //Serial.println("Checked Interupted PIN"); 
  }

  ////prüfen

  
  if ((PIN_OIL_LEVEL == true) && (oil_level == false)){
    check_LED();
  }
  if ((PIN_OIL_LEVEL == false) && (oil_level == true)){
    check_LED();
  }


// Alle 1000 ms: Zeit + GPS
  if (millis() - lastMillis > 1000) {   
    update_time();     
    update_GPS();
    lastMillis = millis();
  }

//handle_KLine();

if (Memory_Button_state != previousMemoryState) {
   // save_Data();  // Speichere alten Set
  currentMemory = Memory_Button_state ? 0 : 1;
   // load_Data();  // Lade neuen Set
    // Passe maxpage an
    // Korrigiere Data.page, falls zu hoch
   if (Data.page > maxpage[currentMemory]) {
      Data.page = 0;
   }
   temp_page = Data.page;
    DrawSelected(Data.page);  // Redraw 
    previousMemoryState = Memory_Button_state;
   check_led = true;
 }

}



void switch_page(void){
  Data.page += 1;
  if (Data.page > maxpage[currentMemory]){  //DEBUG maxpage is set in variables
    Data.page = 0;
  }
  temp_page = Data.page;
  //Page_Switch_Done = true;
  DrawSelected(Data.page);
 //  Serial.println("Button");
  check_LED();
}


// This loop runs on Core 0, while the main loop runs on Core 1
void CAN_Loop(void *parameter) {
  for(;;) {
    if (ESP32Can.readFrame(rxFrame, 0)) {

      #ifdef DEBUG_CAN
//     Serial.printf("CAN ID: 0x%03X | Data: ", rxFrame.identifier);
      for(int i = 0; i < rxFrame.data_length_code; i++) {
//        Serial.printf("%02X ", rxFrame.data[i]);
      }
//      Serial.println();
      #endif

      //  nur erf. IDs  
      switch(rxFrame.identifier) {
        case 0x280:  // Drehzahl
        case 0x288:  // Geschwindigkeit, Tempomat
        case 0x320:  // Tank, Tür
        case 0x420:  // Außentemp, Öl, Licht
        case 0x480:  // Verbrauch (µl)
        case 0x520:  // Kilometerstand
          evaluate_CAN_messages();
          break;
        default:
          break;  // Ignorieren
      }
    }
    vTaskDelay(1);  // CPU Entlasung
  }
}

void ISR_INT_PIN(void){
  PIN_INT_state = digitalRead(INT_PIN);
}
