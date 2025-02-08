// see and edit in variables.h
// int Icon_Pos_Door[2]        = {256, 119};   // position of LED icon    door 
// int Icon_Pos_Light[2]       = {256, 179};   // position of LED icon    light
// int Icon_Pos_Petrol[2]      = {256, 234};   // position of LED icon    petrol
// int Icon_Pos_Oil[2]         = { 14, 119};   // position of LED icon    oil 
// int Icon_Pos_WasherFluid[2] = {256, 179};   // position of LED icon    washer_fluid
// int Icon_Pos_Coolant[2]     = { 14, 234};   // position of LED icon    coolant
// int Icon_Pos_BrakePads[2]   = { 14, 179};   // position of LED icon    brakepads 

void check_LED (void){

// DEBUG:
return;
Serial.println("This is check_LED");
door = false;
light = false;
petrol = false;
oil_level = true;
oil_presure = true;
washer_fluid = true;
coolant = true;
brakepads = true;
brakesystem = true;

warnings = 0;

// DEBUG: 
// The if statement is only for now
if(Data.page == 0){
    // light control // over CAN
    if((light == true) && (TEXT_COLOR != NIGHT_TEXT_COLOR)){
      TEXT_COLOR = NIGHT_TEXT_COLOR;
      DrawSelected(Data.page);
    }
    if((light == false) && (TEXT_COLOR != DAY_TEXT_COLOR)){
      TEXT_COLOR = DAY_TEXT_COLOR;
      DrawSelected(Data.page);
    }

      if(coolant == false){
        tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_coolant, 50, 50, TFT_RED);
        warnings += 1;
      }
      else{
        tft.fillRect(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], 50, 50, BACK_COLOR);
      }

      if(brakesystem == false){
        tft.drawXBitmap(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], sym_brakesystem, 50, 50, TFT_RED);
        warnings += 2;
      }
      else{
        tft.fillRect(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], 50, 50, BACK_COLOR);
      }

      if(petrol == true){
        tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, TFT_ORANGE);
        warnings += 4;
      }
      else{
        tft.fillRect(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], 50, 50, BACK_COLOR);
      }

      if((oil_level == false) || (oil_presure == false)){
        uint16_t COLOR;
        if (oil_level == false){
          COLOR = TFT_ORANGE;
        }
        if(oil_presure == false){
          COLOR = TFT_RED;
        }
        tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, COLOR);
        warnings += 8;
      }
      else{
        tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
      }

      if(brakepads == false){
        tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
        warnings += 16;
      }
      else{
        tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
      }

      if(washer_fluid == false){
        tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, TFT_ORANGE);
        warnings += 32;
      }
      else{
        tft.fillRect(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], 50, 50, BACK_COLOR);
      }

      if(light == true){
        tft.drawXBitmap(Icon_Pos_Light[0], Icon_Pos_Light[1], sym_light, 50, 50, TFT_GREEN);
        warnings += 64;
      }
      else{
        tft.fillRect(Icon_Pos_Light[0], Icon_Pos_Light[1], 50, 50, BACK_COLOR);
      }

      if(door == true){
        tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
        warnings += 128;
      }
      else{
        tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
      }

// DEBUG:
  // // Batterie over voltage measurement
  //     if((volt > 4000) || (volt < 1000)){
  //       tft.drawXBitmap(Icon_Pos_Batterie[0], Icon_Pos_Batterie[1], sym_battery, 50, 50, TFT_RED);
  //       warnings += 256;
  //     }
  //     else {
  //       tft.fillRect(256, 50, 50, 50, BACK_COLOR);
  //     }

// DEBUG:
  Serial.println(warnings);
}

  // Red Alert LED ones need to be placed in Middle and big
  //tft.drawXBitmap(temp + 3 * 50, 50, sym_coolant, 50, 50, TFT_RED);       // Coolant
  //tft.drawXBitmap(temp + 4 * 50, 48, sym_oil, 50, 50, TFT_RED);           // Oil presure
  //tft.drawXBitmap(temp + 4 * 50, 48, sym_brakesystem, 50, 50, TFT_RED);   // Brake fluid level

}

