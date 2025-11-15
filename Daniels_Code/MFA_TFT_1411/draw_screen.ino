void DrawSelected(int page){
  tft.fillScreen(BACK_COLOR);
  tft.setTextDatum(MC_DATUM); // Middle Center
  tft.setTextColor(TEXT_COLOR);
  tft.setFreeFont(FSS18);

  // max 6 info lines
  // Line 1 = 72
  // Line 2 = 138
  // Line 3 = 204
  // Line 4 = 270
  // Line 5 = 337
  // Line 6 = 404

  switch(page){
    
    case 0:
      // DEBUG:
      // This is the visible area
      #ifdef DEBUG_TFT
        tft.drawRect(10, 40, 300, 417, TFT_RED);
      #endif

      // Data.mode = START;
      drawCruiseControl(70);
      //drawVolt(70);
      tft.drawFastHLine(0, 100, 320, TEXT_COLOR);
      tft.drawFastHLine(0, 288, 320, TEXT_COLOR);
      drawRange(316);
      tft.drawFastHLine(0, 344, 320, TEXT_COLOR);
      drawAverageConsumption(372);
      tft.drawFastHLine(0, 400, 320, TEXT_COLOR);
      drawOutTemp(428);
      tft.drawFastHLine(0, 456, 320, TEXT_COLOR);
      break;
    
    case 1:
      // Data.mode = REFUEL;
      drawTrip(72);
      drawActualConsumption(138);
      drawRange(204);
      drawAverageConsumption(270);
      //drawTime(337);
      //drawOutTemp(337);
      drawSpeed(337);
      drawAverageVelocity(404);
      tft.drawString(F("REFUEL"), 160, 453, FONT4);
      break;
    
    case 2:
      // Data.mode = PERIOD;
      drawTrip(72);
      drawActualConsumption(138);
      drawRange(204);
      drawAverageConsumption(270);
      drawTime(337);
      drawAverageVelocity(404);
      tft.drawString(F("PERIOD"), 160, 453, FONT4);
      break;
    
    case 3:
      // show max velocity
      // dtostrf(Data.velocity_max, 3, 0, TFT_String);
      // draw_small_value_box(300, 72, 50, 30, TFT_String);

      // Line 1 = 72  ()
      drawCruiseControl(72);
      // Line 5 = 337 (range)
      drawRange(337);
      // Line 6 = 404 (time)
      // drawTime(404);
      drawOil(404);

      //Draw separation line
      tft.drawFastHLine(0, 105, 320, TEXT_COLOR);
      // tft.drawFastHLine(0, 171, 320, TEXT_COLOR);
      // tft.drawFastHLine(0, 237, 320, TEXT_COLOR);
      tft.drawFastHLine(0, 303, 320, TEXT_COLOR);
      tft.drawFastHLine(0, 369, 320, TEXT_COLOR);
      tft.drawFastHLine(0, 435, 320, TEXT_COLOR);

      break;
    
    case 4:
    // Just draw a half VIS from original Insrument
    // We have only the lower part to draw and 
    // the upper part is used only to display the LED ICON
    // we may display:
    // Trip Time
    // Trip diatance
    // actual consumption
    // average Velocity (distance / time)
    // average consumption (consumption / distance)
    // resting range
    // Out Temp with ice Warning
    // Oil Temp

      drawCruiseControl(70);
      //drawVolt(70);
      tft.drawFastHLine(0, 100, 320, TEXT_COLOR);
      tft.setTextColor(BACK_COLOR, TEXT_COLOR, true);
      if(Data.mode == START){
        tft.drawString(F("1"),300 , 302, FONT4);
      }
      else if (Data.mode == REFUEL){
        tft.drawString(F("2"),300 , 302, FONT4);
      }
      else{
        tft.drawString(F("2"),300 , 302, FONT4);
      }
      tft.setTextColor(TEXT_COLOR, BACK_COLOR, false);
      tft.drawFastHLine(0, 288, 320, TEXT_COLOR);
      drawAverageConsumption(316);
      tft.drawFastHLine(0, 344, 320, TEXT_COLOR);
      drawRange(372);
      tft.drawFastHLine(0, 400, 320, TEXT_COLOR);
      drawOutTemp(428);
      tft.drawFastHLine(0, 456, 320, TEXT_COLOR);
      break;

    case 5:
      // Debug page
      drawDoor();
      break;

// need to store previous page and after getting again K15 switch to that page
    case 6:
      drawShutDown();
      break;
  
    default: 
      break;
  }
}


void drawOil(int Y_Pos){
  // Draw Oil Symbol
  tft.drawXBitmap(19, Y_Pos - 25, sym_oil_temp, 50, 50, TEXT_COLOR);
  drawUnits(Y_Pos, DEGREE);
}

void drawAverageVelocity(int Y_Pos){
  drawUnits(Y_Pos, KM, HOUR);
  draw_average_symbol(44, Y_Pos, 18);
}

void drawCruiseControl(int Y_Pos){
  tft.drawXBitmap(19, Y_Pos - 25, sym_cruise_control, 50, 50, TEXT_COLOR);
  drawUnits(Y_Pos, KM, HOUR);
}

void drawTrip(int Y_Pos){
  drawUnits(Y_Pos, KM);
}

void drawSpeed(int Y_Pos){
  drawUnits(Y_Pos, RPM, MINUTES);
}

void drawRange(int Y_Pos){
  // Draw Petrol symbol
  tft.drawXBitmap(19, Y_Pos - 25, sym_petrol, 50, 50, TEXT_COLOR);
  //Draw units km
  drawUnits(Y_Pos, KM);
}

void drawVolt(int Y_Pos){
  // Draw Battrie symbol
  tft.drawXBitmap(19, Y_Pos - 25, sym_battery, 50, 50, TEXT_COLOR);
  //Draw units km
  drawUnits(Y_Pos, "V");
}

void drawConsumption(int Y_Pos){
  tft.setTextDatum(MC_DATUM);
  tft.drawString(LITER, 220, Y_Pos, FONT4);
}

void drawAverageConsumption(int Y_Pos){
  //Draw Average symbol
  draw_average_symbol(44, Y_Pos, 18);
  drawUnits(Y_Pos, LITER, HUNDERTKM);
} 

void drawActualConsumption(int Y_Pos){
  if(velocity_actual > 5){
    drawUnits(Y_Pos, LITER, HUNDERTKM);
  }
  else{
    drawUnits(Y_Pos, LITER, HOUR);
  }
}

void drawTime(int Y_Pos){
  // Draw hours and / or minutes
  // at beginning we only need minutes, hours will be draw from update_values
  drawUnits(Y_Pos, HOUR);
  tft.drawXBitmap(19, Y_Pos - 25, sym_time, 50, 50, TEXT_COLOR);
}

void drawOutTemp(int Y_Pos){
  //Draw Snowflake only if Temp under 4 degrees
  if (temp_out <= 4){
    tft.drawXBitmap(80, Y_Pos - 25, sym_snowflake, 50, 50, TEXT_COLOR);  // x 19
  }
  else{
    tft.fillRect(80, Y_Pos - 25, 50, 50, BACK_COLOR);   //x 19
  }
  drawUnits(Y_Pos, DEGREE);
}

void drawUnits(int Y_Pos, String upper_line, String lower_line){

  // store X position in variable, so we need only to change this line not several
  // do not use X_Pos, as it is used on other places
  int X = 268;
  // pushing two pixel down looks better for units 
  Y_Pos += 2;

  tft.setTextDatum(MC_DATUM);

  // check if we need the fraction bar
  if (lower_line == ""){
    tft.drawString(upper_line, X, Y_Pos, FONT4);
  }
  else{
    //Draw units km/h km is longer, but U/min min is longer so we need to check how long to draw the line
    if (upper_line.length() == lower_line.length()){
      // this will only happen on units l/h, but than the line is to short, so make it double length
      temp = tft.textWidth(upper_line, FONT4) * 4;
    }
    else if (upper_line.length() > lower_line.length()){
      temp = tft.textWidth(upper_line, FONT4);
    }
    else{
      temp = tft.textWidth(lower_line, FONT4);
    }
    tft.drawString(upper_line, X, Y_Pos - tft.fontHeight(FONT4) / 2, FONT4);
    tft.drawFastHLine(X - temp / 2, Y_Pos - 2, temp, TEXT_COLOR);
    tft.drawString(lower_line, X, Y_Pos + tft.fontHeight(FONT4) / 2, FONT4);
  }
}

void drawDoor(void){
    tft.fillScreen(BACK_COLOR);
    // tft.setTextDatum(MR_DATUM);
    // tft.setFreeFont(FSS12);
    // tft.drawString(String(Data.page), 240, 12, GFXFF);
    tft.drawXBitmap(119, 100, sym_door, 50, 50, TEXT_COLOR);
    tft.setTextDatum(MC_DATUM);
    tft.setFreeFont(FSS18);
    tft.drawString(F("Achtung!"), 134, 25, GFXFF);
    tft.drawString(F("Tuer offen"), 134, 250, GFXFF);
}

// Draw Average symbol
void draw_average_symbol(int X_Pos, int Y_Pos, int Radius){
  tft.fillCircle(X_Pos, Y_Pos, Radius, TEXT_COLOR);
  tft.fillCircle(X_Pos, Y_Pos, Radius - 3, BACK_COLOR);
  tft.drawLine(X_Pos - Radius - 1, Y_Pos + Radius, X_Pos + Radius - 1, Y_Pos - Radius, TEXT_COLOR);
  tft.drawLine(X_Pos - Radius, Y_Pos + Radius, X_Pos + Radius, Y_Pos - Radius, TEXT_COLOR);
  tft.drawLine(X_Pos - Radius + 1, Y_Pos + Radius, X_Pos + Radius + 1, Y_Pos - Radius, TEXT_COLOR);
}

void draw_debug(void){
  // DEBUG
  tft.fillScreen(BACK_COLOR);
  //Draw separation line
  tft.drawFastHLine(0, 105, 320, TEXT_COLOR);
  tft.drawFastHLine(0, 171, 320, TEXT_COLOR);
  tft.drawFastHLine(0, 237, 320, TEXT_COLOR);
  tft.drawFastHLine(0, 303, 320, TEXT_COLOR);
  tft.drawFastHLine(0, 369, 320, TEXT_COLOR);
  tft.drawFastHLine(0, 435, 320, TEXT_COLOR);

  // max 6 info lines
  // Line 1 = 72
  // Line 2 = 138
  // Line 3 = 204
  // Line 4 = 270
  // Line 5 = 337
  // Line 6 = 404
  

  tft.setTextDatum(ML_DATUM);
  tft.drawString("Tank", 14, 72, FONT4 );
  tft.drawString("Oil_T", 14, 138, FONT4 );
  tft.drawString("C_l_25", 14, 204, FONT4 );
  tft.drawString("U/min", 14, 270, FONT4 );
  tft.drawString("C_l_km", 14, 337, FONT4 );
  drawCruiseControl(404);


  // tft.setFreeFont(FSS12);
  // tft.drawString(String(Data.page), 254, 12, GFXFF);
  // tft.setTextDatum(MC_DATUM);
  // tft.drawString(F("DEBUG"),134, 12, FONT4);

  // tft.setTextDatum(ML_DATUM);
  // tft.drawString(F("Deposit"), 14, 1*24+56, FONT4);
  // tft.drawString(F("Time_start"), 14, 2*24+56, FONT4);
  // tft.drawString(F("Temp_oil"), 14, 3*24+56, FONT4);
  // tft.drawString(F("km_Start"), 14, 4*24+56, FONT4);
  // tft.drawString(F("C_25km"), 14, 5*24+56, FONT4);
  // tft.drawString(F("Time_refuel"), 14, 6*24+56, FONT4);
  // tft.drawString(F("Speed"), 14, 7*24+56, FONT4);
  // tft.drawString(F("velocity"), 14, 8*24+56, FONT4);
  // tft.drawString(F("Cruise Contr."), 14, 9*24+56, FONT4);
  // tft.drawString(F("C_last_km"), 14, 10*24+56, FONT4);
  // tft.drawString(F("C_Start"), 14, 11*24+56, FONT4);
  // tft.drawString(F("C_Actual"), 14, 12*24+56, FONT4);
}

void draw_InitPage(void){
    tft.fillScreen(BACK_COLOR);
    tft.pushImage(88, 120, 144, 144, logo);
    // tft.pushImage(90, 98, 141, 300, T4_image);
    tft.setTextDatum(MC_DATUM);;
    tft.drawString(CAR_NO, 160, 340, FONT4);
}

// need to be black to simulate switch off of display
void drawShutDown(void){
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);

  #ifdef DEBUG
    tft.drawString("Waiting for shut Down", 20, 70, FONT2);
  #endif
}
