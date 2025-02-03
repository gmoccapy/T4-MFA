// see and edit in variables.h
// int Icon_Pos_Door[2]        = {256, 119};   // position of LED icon    door 
// int Icon_Pos_Light[2]       = {256, 179};   // position of LED icon    light
// int Icon_Pos_Petrol[2]      = {256, 234};   // position of LED icon    petrol
// int Icon_Pos_Oil[2]         = { 14, 119};   // position of LED icon    oil 
// int Icon_Pos_WasherFluid[2] = {256, 179};   // position of LED icon    washer_fluid
// int Icon_Pos_Coolant[2]     = { 14, 234};   // position of LED icon    coolant
// int Icon_Pos_BrakePads[2]   = { 14, 179};   // position of LED icon    brakepads 

void check_LED (void){

//tft.fillRect(10, 40, 300, 60, TFT_BLACK);
warnings = 0;

// debug infos
door = false;
light = false;
petrol = false;
oil_level = false;
oil_presure = false;
washer_fluid = false;
coolant = false;
brakepads = false;
volt = 2500;

    // door control // over CAN
    if(door == true){
      tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
      warnings += 1;
    }
    else {
      tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
    }

  // light control // over CAN
  if((light == true) && (TEXT_COLOR != NIGHT_TEXT_COLOR)){
    TEXT_COLOR = NIGHT_TEXT_COLOR;
    DrawSelected(Data.page);
    // tft.drawXBitmap(Icon_Pos_Light[0], Icon_Pos_Light[1], sym_light, 50, 50, TFT_GREEN);
    // warnings += 2;
  }
  if((light == false) && (TEXT_COLOR != DAY_TEXT_COLOR)){
    TEXT_COLOR = DAY_TEXT_COLOR;
    DrawSelected(Data.page);
    // tft.fillRect(Icon_Pos_Light[0], Icon_Pos_Light[1], 50, 50, BACK_COLOR);
  }

  // petrol warning // over CAN
  if(petrol == true){
    tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, TFT_ORANGE);
    warnings += 4;
  }
  else {
    tft.fillRect(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], 50, 50, BACK_COLOR);
  }

  // oil warnings // over IO
  if(oil_level == true){
    tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_ORANGE);
    warnings += 8;
  }
  else {
    tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
  }

  // oil warnings // over IO
  // if(oil_presure == true){
  //   tft.drawXBitmap(256, 179, sym_oil, 50, 50, TFT_RED);
  //   warnings += 16;
  // }
  // else {
  //   tft.fillRect(256, 179, 50, 50, BACK_COLOR);
  // }

  // brake pads waring // over IO
  if(washer_fluid == true){
    tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, TFT_ORANGE);
    warnings += 32;
  }
  else {
    tft.fillRect(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], 50, 50, BACK_COLOR);
  }

  // brake pads waring // over IO
  if(brakepads == true){
    tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
    warnings += 64;
  }
  else {
    tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
  }

  // coolant warning // over IO
  if(coolant == true){
    tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_brakepads, 50, 50, TFT_RED);
    warnings += 128;
  }
  else {
    tft.fillRect(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], 50, 50, BACK_COLOR);
  }

  // Batterie over voltage measurement
  if((volt > 4000) || (volt < 1000)){
    tft.drawXBitmap(256, 50, sym_battery, 50, 50, TFT_RED);
    warnings += 256;
  }
  else {
    tft.fillRect(256, 50, 50, 50, BACK_COLOR);
  }

  Serial.println(warnings);
  Serial.println(light);
  
}

