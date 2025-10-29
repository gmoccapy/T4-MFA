void check_IO(){ // int PIN, bool mcp_state){


// DEBUG : There shoud never be print statements on interupt routines!
//         We need to delete them at the end of testing
  warnings = 0;

  // as we do not use MCP PINS 14 and 15 we only iterate to 13
  // as MCP PIN 11 ; 12 and 13 are handling the control button, we will handle them separate
  for (byte i = 0; i < 11; i++){
    // MCP PIN 6 and 7 are not used
    if ((i == 6) || (i == 7)){
      continue;
    }
    if (mcp.digitalRead(i) == true){
      warnings += pow(2, i);
//      Serial.println(i);
    }
  }

  // this is the control button handling
  for (byte i = 11; i < 14; i++){
    switch (i){
      // Mode button
      case 11:
        if (mcp.digitalRead(i) == true){
          //Serial.println("Mode Button");
          Mode_Button_pressed = millis();
        }
        else{
          Mode_Button_pressed = 0;
        }
        break;

// ToDo : Implement memory button ; will switch between setup and operation
      // Memory button
      case 12:
        if (mcp.digitalRead(i) == true){
          //Serial.println("Memory Button");
        //   Memory_Button_pressed = millis();
        }
        // else{
        //   Memory_Button_pressed = 0;
        // }
        break;

      // Reset button
      case 13:
        if (mcp.digitalRead(i) == true){
          Reset_Button_pressed = millis();
          //Serial.println("Reset Button");
        }
        else{
          Reset_Button_pressed = 0;
        }
        break;
    }
  }

  // Serial.print("warnings = ");
  // Serial.println(warnings);

  check_led = true;

  mcp.clearInterrupts();  // clear
  PIN_INT_state = true;
}
