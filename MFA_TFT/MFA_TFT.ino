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

bool DEBUG = true;

void setup(void) {

  if(DEBUG){
    Serial.begin(115200);
  }

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

  if (reset == true){
    reset_Data(Data.mode);
    //avoid page change
    reset = false;
  }

  // initial drawing the screen
  if(start == false){
    // draw the Starting image
    draw_InitPage();
    delay(2000);

    DrawSelected(Data.page);
    start = true;
    if(DEBUG){
      Serial.println("Start");
    }
    check_led = true;
    
    // We do an initial check for the mccp state, as we may have unatended IO's
    check_IO();
  }

  // update time every second
  if (millis() - lastMillis > 1000){
    update_time();
    lastMillis = millis();
    if(DEBUG){
      Serial.print("ShutDownTimer = ");
      Serial.print(shutdown_timer);
      Serial.print("\t");
      Serial.print("Volt = ");
      Serial.print(volt);
      Serial.print("\t");
      Serial.print("Stay On Status = ");
      Serial.print(digitalRead(PIN_STAY_ON));
      Serial.print("\t");
      Serial.print("Data.page / temp_page = ");
      Serial.print(Data.page);
      Serial.print(" / ");
      Serial.println(temp_page);
    }
  }

// DEBUG : Check LED behavior
  // motor_on = true;
  // petrol = true;
  // light = true;
  // batterie = true;
  // brakepads = true;
  // brakesystem = true;
  // washer_fluid = true;
  // oil_presure = true;
  // door = true;
  // coolant = true;
  // warnings = 511;

  // let the symbol blink, so we do not need a special place for the LED
  // can not be done in check LED, as it will lead to a blinking dial in this case line is 316
  // and should not been done if shutdown timer is running
  if (shutdown_timer == 0){
    if(Data.time_start % 2 != 0){
      temp_color = TFT_ORANGE;
    }
    else{
      temp_color = TEXT_COLOR;
    }

    if(petrol == true){
      tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, temp_color);
    }
    else if ((petrol == false) && (temp_color == TFT_ORANGE)){
      tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, TEXT_COLOR);
    }

    if(temp_out_warning = true){
      //Draw Snowflake only if Temp under 4 degrees
      tft.drawXBitmap(19, 428 - 25, sym_snowflake, 50, 50, temp_color);
    }
    else if ((temp_out_warning == false) && (temp_color == TFT_ORANGE)){
        tft.drawXBitmap(19, 428 - 25, sym_snowflake, 50, 50, TEXT_COLOR);
    }
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
  if ((shutdown_timer != 0) && (millis() > shutdown_timer + StayOnTime)){
    digitalWrite(PIN_STAY_ON, LOW);
   }

  if((Mode_Button_pressed != 0) && (millis() > Mode_Button_pressed + 200)){
    Data.mode += 1;
    if (Data.mode > 2){
      Data.mode = 0;
    }
    Mode_Button_pressed = 0;
  }

// ToDo: Here must be introduced the code for the memory switch
// Final behavior should be:
// Mode Button togles the modes (from start, sice refuel and perion) if Memory switch is on 1 (Operation Mode)
// Mode button selects the line to be editided if Memory is on 2 (Setup Mode)
// Short Reset push should toggle between posible values to be displayed in that line. If dial is selected, 
// it might be possible to hide dial and selct up to 3 additional values as line displays 


  if((Reset_Button_pressed != 0) && (millis() > Reset_Button_pressed + 3000)){
    reset = true;
    Reset_Button_pressed = 0;
  }

  if (!PIN_INT_state){
    check_IO();
    //Serial.println("Checked Interupted PIN"); 
  }

  if (((digitalRead(PIN_OIL_LEVEL) == HIGH) && (oil_level == false))||((digitalRead(PIN_OIL_LEVEL) == LOW) && (oil_level == true))){
    check_LED();
  }
  
}

void switch_page(void){
  Data.page += 1;
  if (Data.page > 4){
    Data.page = 0;
  }
  temp_page = Data.page;
  //Page_Switch_Done = true;
  DrawSelected(Data.page);
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

