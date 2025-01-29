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
    if(Data.page == 3){
      tft.drawXBitmap(Icon_Pos_Door[0], Icon_Pos_Door[1], sym_door, 50, 50, TFT_ORANGE);
    }
    else{
      temp_page = 5;
      DrawSelected(temp_page);
    }
  }
  else {
    if(Data.page == 3){
      tft.fillRect(Icon_Pos_Door[0], Icon_Pos_Door[1], 50, 50, BACK_COLOR);
    }
    else{
      temp_page = Data.page;
      DrawSelected(Data.page);
    }
  }

  // light control // over CAN
  if((light == true) && (TEXT_COLOR != NIGHT_TEXT_COLOR)){
    if(Data.page == 3){
      tft.drawXBitmap(Icon_Pos_Light[0], Icon_Pos_Light[1], sym_light, 50, 50, TFT_GREEN);
    }
    TEXT_COLOR = NIGHT_TEXT_COLOR;
    DrawSelected(Data.page);
  }
  if((light == false) && (TEXT_COLOR != DAY_TEXT_COLOR)){
    if(Data.page == 3){
      tft.fillRect(Icon_Pos_Light[0], Icon_Pos_Light[1], 50, 50, BACK_COLOR);
    }
    TEXT_COLOR = DAY_TEXT_COLOR;
    DrawSelected(Data.page);
  }

  // petrol warning // over CAN
  if(petrol == true){
    if(Data.page == 3){
      tft.drawXBitmap(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], sym_petrol, 50, 50, TFT_ORANGE);
    }
  }
  else {
    if(Data.page == 3){
      tft.fillRect(Icon_Pos_Petrol[0], Icon_Pos_Petrol[1], 50, 50, BACK_COLOR);
    }
  }

  // oil warnings // over IO
  if(oil_level == false){
    if(Data.page == 3){
      tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_ORANGE);
    }
  }
  else {
    if(Data.page == 3){
      tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
    }
  }

  // oil warnings // over IO
  if(oil_presure == false){
    if(Data.page == 3){
      tft.drawXBitmap(135, 225, sym_oil, 50, 50, TFT_RED);
    }
  }
  else {
    if(Data.page == 3){
      tft.fillRect(135, 255, 50, 50, BACK_COLOR);
    }
  }


  counter += 1;

  check_led = false;

}
