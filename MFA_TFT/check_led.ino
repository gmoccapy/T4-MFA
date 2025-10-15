// see and edit in variables.h
// int Icon_Pos_Coolant[2]     = { 20,  50};   // position of LED icon    coolant      red
// int Icon_Pos_BrakeSystem[2] = {135,  50};   // position of LED icon    brakepads    orange
// int Icon_Pos_Petrol[2]      = {240,  50};   // position of LED icon    petrol       orange
// int Icon_Pos_Oil[2]         = { 20, 140};   // position of LED icon    oil          red / orange
// int Icon_Pos_BrakePads[2]   = {135, 140};   // position of LED icon    brakepads    red
// int Icon_Pos_WasherFluid[2] = {240, 140};   // position of LED icon    washer_fluid orange
// int Icon_Pos_Light[2]       = { 20, 210};   // position of LED icon    light        green
// int Icon_Pos_Door[2]        = {135, 210};   // position of LED icon    door         orange
// int Icon_Pos_Batterie[2]    = {240, 210};   // position of LED icon    batterie     red

// Red Alert LED ones need to be placed in Middle and big
// tft.drawXBitmap(temp + 3 * 50, 50, sym_coolant, 50, 50, TFT_RED);       // Coolant
// tft.drawXBitmap(temp + 4 * 50, 48, sym_oil, 50, 50, TFT_RED);           // Oil presure
// tft.drawXBitmap(temp + 4 * 50, 48, sym_brakesystem, 50, 50, TFT_RED);   // Brake fluid level

// DEBUG: no place for light symbol, light on/off signaled by TEXT_COLOR

      // if(light == true){
      //   tft.drawXBitmap(Icon_Pos_Light[0], Icon_Pos_Light[1], sym_light, 50, 50, TFT_GREEN);
      // }
      // else{
      //   tft.fillRect(Icon_Pos_Light[0], Icon_Pos_Light[1], 50, 50, BACK_COLOR);
      // }

      
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

  // light control // over CAN
  if((light == true) && (TEXT_COLOR != NIGHT_TEXT_COLOR)){
    TEXT_COLOR = NIGHT_TEXT_COLOR;
    DrawSelected(Data.page);
  }
  if((light == false) && (TEXT_COLOR != DAY_TEXT_COLOR)){
    TEXT_COLOR = DAY_TEXT_COLOR;
    DrawSelected(Data.page);
  }

  if(Data.page == 0){

    tft.drawRect(51, 107, 217, 173, TFT_RED);
    tft.fillRect(52, 108, 215, 171, BACK_COLOR);

    // door from CAN message
    if(door == true){
      tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
    }
    else{
      tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
    }

    // Batterie over/under voltage from measurement in update values
    if (batterie == true){
      tft.drawXBitmap(Icon_Pos_Batterie[0], Icon_Pos_Batterie[1], sym_battery, 50, 50, TFT_RED);
    }
    else {
      tft.fillRect(Icon_Pos_Batterie[0], Icon_Pos_Batterie[1], 50, 50, BACK_COLOR);
    }

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

    // Oil Presure
      if ((i == 8) && (io_state == true)){
        tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_RED);
      }
      else if ((i == 8) && (io_state == false)){
        tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
      }

    // coolant
      if((i == 9) && (io_state == true)){
        tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_coolant, 50, 50, TFT_RED);
      }
      else if ((i == 9) && (io_state == false)){
        tft.fillRect(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], 50, 50, BACK_COLOR);
      }

    // brakepads
      if((i == 0) && (io_state == true)){
        tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
      }
      else if ((i == 0) && (io_state == false)){
        tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
      }

// ToDo : We need Symbols for this (idear: Initioal grafic with colored motor cap)
    // motor_cap
      if((i == 1) && (io_state == true)){
        tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
      }
      else if ((i == 1) && (io_state == false)){
        tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
      }

    // washer fluid
      if((i == 3) && (io_state == true)){
        tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, TFT_ORANGE);
      }
      else if ((i == 3) && (io_state == false)){
        tft.fillRect(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], 50, 50, BACK_COLOR);
      }

    // brakesystem 
      if((i == 2) && (io_state == true)){
        tft.drawXBitmap(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], sym_brakesystem, 50, 50, TFT_RED);
      }
      else if ((i == 2) && (io_state == false)){
        tft.fillRect(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], 50, 50, BACK_COLOR);
      }
    }
  }
}

