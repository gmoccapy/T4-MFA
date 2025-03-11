void check_IO(int PIN, bool mcp_state){

  switch(PIN){
    
    case PIN_MODE:
      if (mcp_state == true){               // releasing the button will change the page
        Mode_Button_pressed = 0;
        if (Page_Switch_Done == false){
          switch_page();
        }
      }
      else{
        Mode_Button_pressed = millis();
        if (Page_Switch_Done == true){
          Page_Switch_Done = false;
        }
      }
      break;

    case PIN_MEMORY:
      Serial.println("PIN_MEMORY IO has changed");
      break;

    case PIN_BRAKEPADS:
      Serial.println("PIN_BRAKEPADS IO has changed");
      if(mcp_state == false){
        tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
      }
      break;
  
    case PIN_BRAKESYSTEM:
      Serial.println("PIN_BRAKESYSTEM IO has changed");
      if(mcp_state == false){
        tft.drawXBitmap(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], sym_brakesystem, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_WASHER_FLUID:
      Serial.println("PIN_WASHER_FLUID IO has changed");
      if(mcp_state == false){
        tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_OIL_LEVEL:
      Serial.println("PIN_OIL_LEVEL IO has changed");
      if(mcp_state == false){
        tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_OIL_PRESURE:
      Serial.println("PIN_OIL_PRESURE IO has changed");
      if(mcp_state == false){
        tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_COOLANT:
      Serial.println("PIN_COOLANT IO has changed");
      if(mcp_state == false){
        tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_coolant, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], 50, 50, BACK_COLOR);
      }

    case PIN_R_DOOR:
      Serial.println("PIN_R_DOOR IO has changed");
      
      break;

    case PIN_S_DOOR:
      Serial.println("PIN_S_DOOR IO has changed");
      
      break;

    case PIN_TRUNK:
      Serial.println("PIN_TRUNK IO has changed");
      
      break;

  }

  mcp.clearInterrupts();  // clear
  PIN_INT_state = true;
// DEBUG :
  Serial.print("Got an Interupt from Pin");
  Serial.print(" ");
  Serial.print(PIN);
  Serial.print(" with state = ");
  Serial.println(mcp_state);
  Serial.println(Mode_Button_pressed);
}