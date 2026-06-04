// see and edit in variables.h
// int Icon_Pos_Coolant[2]     = { 20,  50};   // position of LED icon    coolant      red
// int Icon_Pos_BrakeSystem[2] = {135,  50};   // position of LED icon    brakepads    orange
// int Icon_Pos_Petrol[2]      = {240,  50};   // position of LED icon    petrol       orange
// int Icon_Pos_Oil[2]         = { 20, 140};   // position of LED icon    oil          red / orange
// int Icon_Pos_BrakePads[2]   = {135, 140};   // position of LED icon    brakepads    red
// int Icon_Pos_WasherFluid[2] = {240, 140};   // position of LED icon    washer_fluid orange
// int Icon_Pos_Light[2]       = { 20, 210};   // position of LED icon    light        green
// int Icon_Pos_Door[2]        = {135, 210};   // position of LED icon    door         orange
// int Icon_Pos_Battery[2]    = {240, 210};   // position of LED icon    battery    red

// Red Alert LED ones need to be placed in Middle and big
// tft.drawXBitmap(temp + 3 * 50, 50, sym_coolant, 50, 50, TFT_RED);       // Coolant
// tft.drawXBitmap(temp + 4 * 50, 48, sym_oil, 50, 50, TFT_RED);           // Oil pressure
// tft.drawXBitmap(temp + 4 * 50, 48, sym_brakesystem, 50, 50, TFT_RED);   // Brake fluid level

      // see main loop for blinking LED
      // if(petrol == true){
      //   if(Data.time_start % 2 == 0){
      //     tft.drawXBitmap(19, 316 - 25, sym_petrol, 50, 50, TFT_ORANGE);
      //   }
      //   else{
      //     tft.drawXBitmap(19, 316 - 25, sym_petrol, 50, 50, TEXT_COLOR);
      //   }
      // }
      // else{
      //   tft.drawXBitmap(19, 316 - 25, sym_petrol, 50, 50, TEXT_COLOR);
      // }

void check_LED (void){
//  Serial.println("this is check LED"); 

if (shutdown_timer != 0) {
    return;  // Während Shutdown nichts machen!
  }

  // light control // over CAN
  if((light == true) && (TEXT_COLOR != NIGHT_TEXT_COLOR)){
    TEXT_COLOR = NIGHT_TEXT_COLOR;
    DrawSelected(Data.page);
  }
  if((light == false) && (TEXT_COLOR != DAY_TEXT_COLOR)){
    TEXT_COLOR = DAY_TEXT_COLOR;
    DrawSelected(Data.page);
  }

  if((currentMemory == 0) && (Data.page == 0)){
    
#ifdef DEBUG_TFT
    tft.drawRect(51, 107, 217, 173, TFT_RED);
#endif

    tft.fillRect(52, 108, 215, 171, BACK_COLOR);

#ifdef SYM_NA_VISIBLE
  if (light){
    tft.drawXBitmap(Icon_Pos_Light[0], Icon_Pos_Light[1], sym_light, 50, 50, LIGHT_GREEN);
     } 
  else{
    tft.drawXBitmap(Icon_Pos_Light[0], Icon_Pos_Light[1], sym_light, 50, 50, SYM_NA_COLOR);
    //tft.fillRect(Icon_Pos_Light[0], Icon_Pos_Light[1], 50, 50, BACK_COLOR);
     }
#endif



    // door from CAN message
    if(door == true){
      tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
    }
    else{
       #ifdef SYM_NA_VISIBLE
      tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, SYM_NA_COLOR);  
      #else
      tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
      #endif
    }


#ifdef SYM_NA_VISIBLE
//nichts
#else
  // Batterie over/under voltage from measurement in update values        
  if (battery == true){
  tft.drawXBitmap(Icon_Pos_Battery[0], Icon_Pos_Battery[1], sym_battery, 50, 50, TFT_RED);
   }
    else {
    tft.fillRect(Icon_Pos_Battery[0], Icon_Pos_Battery[1], 50, 50, BACK_COLOR);
   }
#endif
    //if (oil_level == true){ //Wird zusammen mit oil_pressure geprüft
    //    tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_ORANGE);
    //}
    //else if (oil_level == false){
    //  tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
    //}

    // this are the MCP connected IO's
    // as we do not use MCP PINS 14 and 15 we only iterate to 13
    for (byte i = 0; i < 14; i++){
      // MCP PIN 6 and 7 are not used
      if ((i == 6) || (i == 7)){
        continue;
      }
      bool io_state = bitRead(warnings, i);
      // Serial.print(io_state);
      // Serial.print("\t");


//Öl LED -> Pin 4 orange, Pin 8 rot
if ((i == 4) && (io_state== true)){  // PIN 4 is used for the orange oil LED
oil_level = false;
} else {
  oil_level = true;
}

 if ((i == 8)) {
uint16_t oil_color = SYM_NA_COLOR;  // Default: nichts anzeigen

    // 1. Ölstand zu niedrig → ORANGE (sofort)
    if (oil_level == false) {
      oil_color = TFT_ORANGE;
    }

    if (bitRead(warnings, i) == 1) {  // Pin 8 = LOW → Warnung aktiv
        oil_color = TFT_RED;  // ROT hat immer Vorrang!
      }
    if (oil_color != SYM_NA_COLOR) {
      tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, oil_color);
      oil_pressure = false;

    } else {

      #ifdef SYM_NA_VISIBLE
      tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, SYM_NA_COLOR);  
      #else
      tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
      #endif
      oil_pressure = true;
    }
 }
    
    // coolant   // Logik umkehr wegen Auswertung LED
      if((i == 9) && (io_state == true)){
        coolant = false;
        tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_coolant, 50, 50, TFT_RED);
      }
      else if ((i == 9) && (io_state == false)){
        coolant = true;
      
      #ifdef SYM_NA_VISIBLE
      tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_coolant, 50, 50, SYM_NA_COLOR);  
      #else
      tft.fillRect(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], 50, 50, BACK_COLOR);
      #endif 
      }

    // brakepads (circuit normaly closed)
      if((i == 0) && (io_state == false)){
        brakepads = false;
         tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
       }
      else if ((i == 0) && (io_state == true)){
      #ifdef SYM_NA_VISIBLE
      tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, SYM_NA_COLOR);  
      #else
      tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
      #endif
         brakepads = true;
       }

// ToDo : We need Symbols for this (idea: initioal grafic with colored motor cap)
//        has been realized by Daniel Braun, thanks to him I will be able to implement that in a future release
    
    // motor_cap
//      if((i == 1) && (io_state == true)){
//        tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
//      }
//      else if ((i == 1) && (io_state == false)){
//        tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
//      }

    // washer fluid
      if((i == 3) && (io_state == true)){
        tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, TFT_ORANGE);
      washer_fluid = true;
      }
      else if ((i == 3) && (io_state == false)){
      washer_fluid = false;
      #ifdef SYM_NA_VISIBLE
       tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, SYM_NA_COLOR);  
      #else
      tft.fillRect(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], 50, 50, BACK_COLOR);
      #endif
        }

    // brakesystem 
      if((i == 2) && (io_state == true)){
        brakesystem = false;
        tft.drawXBitmap(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], sym_brakesystem, 50, 50, TFT_RED);
      }
      else if ((i == 2) && (io_state == false)){
        brakesystem = true;
      #ifdef SYM_NA_VISIBLE
       tft.drawXBitmap(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], sym_brakesystem, 50, 50, SYM_NA_COLOR);  
      #else
      tft.fillRect(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], 50, 50, BACK_COLOR);
      #endif
       }
 
  //tft.fillRect(Icon_Pos_Warn[0], Icon_Pos_Warn[1], 50, 50, BACK_COLOR);   // rotes Warnsymbol unten neben Schneeflocke
   
    #ifdef SYM_NA_VISIBLE
       tft.drawXBitmap(Icon_Pos_Warn[0], Icon_Pos_Warn[1], sym_warn2, 50, 50, SYM_NA_COLOR);  
      #else
      tft.fillRect(Icon_Pos_Warn[0], Icon_Pos_Warn[1], 50, 50, BACK_COLOR);
      #endif


  if ((oil_level == false) || (oil_pressure == false) || (coolant == false) || (brakepads == false)|| (brakesystem == false)){

 
  tft.drawXBitmap(Icon_Pos_Warn[0], Icon_Pos_Warn[1], sym_warn2, 50, 50, TFT_RED);
  }

    }
  }
}
