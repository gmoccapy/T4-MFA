// see and edit in variables.h
// int Icon_Pos_Door[2]        = {256, 119};   // position of LED icon    door 
// int Icon_Pos_Light[2]       = {256, 179};   // position of LED icon    light
// int Icon_Pos_Petrol[2]      = {256, 234};   // position of LED icon    petrol
// int Icon_Pos_Oil[2]         = { 14, 119};   // position of LED icon    oil 
// int Icon_Pos_WasherFluid[2] = {256, 179};   // position of LED icon    washer_fluid
// int Icon_Pos_Coolant[2]     = { 14, 234};   // position of LED icon    coolant
// int Icon_Pos_BrakePads[2]   = { 14, 179};   // position of LED icon    brakepads 

void check_LED (void){

    // door control // over CAN
    if(door == true){
      tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
    }
    else {
      tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
    }

  // light control // over CAN
  if((light == true) && (TEXT_COLOR != NIGHT_TEXT_COLOR)){
    tft.drawXBitmap(Icon_Pos_Light[0], Icon_Pos_Light[1], sym_light, 50, 50, TFT_GREEN);
    TEXT_COLOR = NIGHT_TEXT_COLOR;
    DrawSelected(Data.page);
  }
  if((light == false) && (TEXT_COLOR != DAY_TEXT_COLOR)){
    tft.fillRect(Icon_Pos_Light[0], Icon_Pos_Light[1], 50, 50, BACK_COLOR);
    TEXT_COLOR = DAY_TEXT_COLOR;
    DrawSelected(Data.page);
  }

  // petrol warning // over CAN
  if(petrol == true){
    tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, TFT_ORANGE);
  }
  else {
    tft.fillRect(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], 50, 50, BACK_COLOR);
  }

  // oil warnings // over IO
  if(oil_level == false){
    tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_ORANGE);
  }
  else {
    tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
  }

  // oil warnings // over IO
  if(oil_presure == false){
    tft.drawXBitmap(256, 179, sym_oil, 50, 50, TFT_RED);
  }
  else {
    tft.fillRect(256, 179, 50, 50, BACK_COLOR);
  }

  // brake pads waring // over IO
  if(washer_fluid == false){
    tft.drawXBitmap(256, 179, sym_washer_fluid, 50, 50, TFT_ORANGE);
  }
  else {
    tft.fillRect(256, 179, 50, 50, BACK_COLOR);
  }

  // brake pads waring // over IO
  if(brakepads == false){
    tft.drawXBitmap(14, 179, sym_brakepads, 50, 50, TFT_ORANGE);
  }
  else {
    tft.fillRect(14, 179, 50, 50, BACK_COLOR);
  }

  // coolant warning // over IO
  if(coolant == false){
    tft.drawXBitmap(14, 234, sym_brakepads, 50, 50, TFT_RED);
  }
  else {
    tft.fillRect(14, 234, 50, 50, BACK_COLOR);
  }

  // Batterie over voltage measurement
  if((volt > 4000) || (volt < 1000)){
    tft.drawXBitmap(14, 234, sym_battery, 50, 50, TFT_RED);
  }
  else {
    tft.fillRect(14, 234, 50, 50, BACK_COLOR);
  }


}

