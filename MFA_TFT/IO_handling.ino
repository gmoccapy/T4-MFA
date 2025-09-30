void check_IO(int PIN, bool mcp_state){
  
  switch(PIN){
  
    case PIN_MODE:
      Serial.print("PIN_MODE IO has changed to ");
      Serial.println(mcp_state);
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
      Serial.print("PIN_MEMORY IO has changed to ");
      Serial.println(mcp_state);
      break;

    case PIN_BRAKEPADS:
      Serial.print("PIN_BRAKEPADS IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_BRAKESYSTEM:
      Serial.print("PIN_BRAKESYSTEM IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        tft.drawXBitmap(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], sym_brakesystem, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_WASHER_FLUID:
      Serial.print("PIN_WASHER_FLUID IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_OIL_LEVEL:
      Serial.print("PIN_OIL_LEVEL IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_OIL_PRESURE:
      Serial.print("PIN_OIL_PRESURE IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_COOLANT:
      Serial.println("PIN_COOLANT IO has changed to ");
      Serial.print(mcp_state);
      if(mcp_state == true){
        tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_coolant, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], 50, 50, BACK_COLOR);
      }

    case PIN_R_DOOR:
      Serial.print("PIN_R_DOOR IO has changed to");
      Serial.println(mcp_state);
      
      break;

    case PIN_S_DOOR:
      Serial.print("PIN_S_DOOR IO has changed to ");
      Serial.println(mcp_state);
    
      break;

    case PIN_TRUNK:
      Serial.print("PIN_TRUNK IO has changed to ");
      Serial.println(mcp_state);
    
      break;

    default: break;
  }

  mcp.clearInterrupts();  // clear
  PIN_INT_state = true;
}