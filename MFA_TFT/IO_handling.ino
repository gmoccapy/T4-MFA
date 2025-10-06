void check_IO(int PIN, bool mcp_state){


// DEBUG : There shoud never be print statements on interupt routines!
//         We need to delete them at the end of testing
  warnings = 0;


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

    case PIN_RESET:
      Serial.print("PIN_RESET IO has changed to ");
      Serial.println(mcp_state);

      break;

    case PIN_TRUNK:
      Serial.print("PIN_TRUNK IO has changed to ");
      Serial.println(mcp_state);
      warnings += 1;

      break;

    case PIN_R_DOOR:
      Serial.print("PIN_R_DOOR IO has changed to");
      Serial.println(mcp_state);
      warnings += 2;

      break;

    case PIN_S_DOOR:
      Serial.print("PIN_S_DOOR IO has changed to ");
      Serial.println(mcp_state);
      warnings += 4;

      break;

    case PIN_OIL_PRESURE:
      Serial.print("PIN_OIL_PRESURE IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        warnings += 8;
        oil_presure = true;
      }
      else{
        oil_presure = false;
      }

      break;

    case PIN_COOLANT:
      Serial.print("PIN_COOLANT IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        warnings += 16;
        tft.drawXBitmap(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], sym_coolant, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_Coolant[0], Icon_Pos_Coolant[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_BRAKEPADS:
      Serial.print("PIN_BRAKEPADS IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        warnings += 32;
        tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_MOTOR_CAP:
      Serial.print("PIN_MOTOR_CAP IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        warnings += 64;
      //   tft.drawXBitmap(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], sym_brakepads, 50, 50, TFT_ORANGE);
      }
      // else{
      //   tft.fillRect(Icon_Pos_BrakePads[0], Icon_Pos_BrakePads[1], 50, 50, BACK_COLOR);
      // }
      break;

    case PIN_WASHER_FLUID:
      Serial.print("PIN_WASHER_FLUID IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        warnings += 128;
        tft.drawXBitmap(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], sym_washer_fluid, 50, 50, TFT_ORANGE);
      }
      else{
        tft.fillRect(Icon_Pos_WasherFluid[0], Icon_Pos_WasherFluid[1], 50, 50, BACK_COLOR);
      }
      break;

    case PIN_BRAKESYSTEM:
      Serial.print("PIN_BRAKESYSTEM IO has changed to ");
      Serial.println(mcp_state);
      if(mcp_state == true){
        warnings += 256;
        tft.drawXBitmap(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], sym_brakesystem, 50, 50, TFT_RED);
      }
      else{
        tft.fillRect(Icon_Pos_BrakeSystem[0], Icon_Pos_BrakeSystem[1], 50, 50, BACK_COLOR);
      }
      break;


// DEBUG INFO . This need to be done in a separate way, as we will get an PWN Signal from sensor
    // case PIN_OIL_LEVEL: 
    //   Serial.print("PIN_OIL_LEVEL IO has changed to ");
    //   Serial.println(mcp_state);
    //   if(mcp_state == true){
    //     tft.drawXBitmap(Icon_Pos_Oil[0], Icon_Pos_Oil[1], sym_oil, 50, 50, TFT_ORANGE);
    //   }
    //   else{
    //     tft.fillRect(Icon_Pos_Oil[0], Icon_Pos_Oil[1], 50, 50, BACK_COLOR);
    //   }
    //   break;

// DEBUG INFO: OIL_LED / showing low oil level or old oil, this one is connected direktly to
// PIN 11 of ESP32, that correspond to GPIO 25
// Attantion, do not apply 12 V to this PIN, as it will destoy ESP32


    default:break;
  }

  check_led = true;

  mcp.clearInterrupts();  // clear
  PIN_INT_state = true;
}