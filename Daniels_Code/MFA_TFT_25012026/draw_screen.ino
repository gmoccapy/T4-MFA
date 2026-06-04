#define UNIT_X 230
#define UNIT_FONT FONT4

void DrawSelected(int page){
  create_sprites();
  tft.fillScreen(BACK_COLOR);
  tft.setTextDatum(MC_DATUM); // Middle Center
  tft.setTextColor(TEXT_COLOR);
  tft.setFreeFont(FSS18);

  // max 6 info lines - 66er  Abstand
  // Line 1 = 72
  // Line 2 = 138
  // Line 3 = 204
  // Line 4 = 270
  // Line 5 = 337
  // Line 6 = 404

// Shutdown-Seite (6) unabhängig vom Memory
  if (shutdown_timer != 0 || page == 6) {
    drawShutDown();
    temp_page = 6;
    return;
  }

  // Normale Seiten – abhängig von currentMemory
  if (currentMemory == 0) {
    switch(page) {
      case 0:
        
        #ifdef DEBUG_TFT
          tft.drawRect(10, 40, 300, 417, TFT_RED);   //This is the visible area
        #endif
        drawCruiseControl(70);
        tft.drawFastHLine(0, 100, 320, TEXT_COLOR);
        tft.setTextColor(BACK_COLOR, TEXT_COLOR, true);
        if(Data.mode == START){
          tft.drawString(F("1"),300 , 302, FONT4);
        }
        else if (Data.mode == REFUEL){
          tft.drawString(F("2"),300 , 302, FONT4);
        }
        else{
          tft.drawString(F("3"),300 , 302, FONT4);
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

      case 1:
        
        drawTrip(72);
        drawActualConsumption(138);
        drawDeposit(204);
        drawAverageConsumption(270);
        drawTime(337);
        drawAverageVelocity(404);
     
        if(Data.mode == START){
          tft.drawString(F("seit Start"), 160, 448, FONT4);
        }
        else if (Data.mode == REFUEL){
          tft.drawString(F("seit Tanken"), 160, 448, FONT4);
        }
        else if (Data.mode == PERIOD){
          tft.drawString(F("Langzeit"), 160, 448, FONT4);
        }
        break;

      default:
        break;
    }
  } else {  // currentMemory == 1
    switch(page) {
      
      case 0:
      
      break;
      
      case 1:
        
        drawCruiseControl(70); //72
        drawUMin(337);
        drawOil(404);
        //Draw separation lines
        tft.drawFastHLine(0, 105, 320, TEXT_COLOR);
        tft.drawFastHLine(0, 303, 320, TEXT_COLOR);
        tft.drawFastHLine(0, 369, 320, TEXT_COLOR);
        tft.drawFastHLine(0, 435, 320, TEXT_COLOR);
        break;

      default:
        break;
    }
  }
}

void drawOil(int Y_Pos){
  // Draw Oil Symbol
  tft.drawXBitmap(19, Y_Pos - 25, sym_oil_temp, 50, 50, TEXT_COLOR);
  drawUnits(Y_Pos, DEGREE);
}

// void drawOilLevel(int Y_Pos){
//  tft.drawXBitmap(19, Y_Pos - 25, sym_oil, 50, 50, TEXT_COLOR);  // anderes Symbol!
//  drawUnits(Y_Pos, F("mm"));  // ← mm
//}

void drawAverageVelocity(int Y_Pos){
  drawUnits(Y_Pos, KM, HOUR);
  draw_average_symbol(44, Y_Pos, 18);
}

void drawCruiseControl(int Y_Pos){
  tft.drawXBitmap(19, Y_Pos - 25, sym_cruise_control, 50, 50, TEXT_COLOR);
  drawUnits(Y_Pos, KM, HOUR);
}

void drawTrip(int Y_Pos){
  tft.drawXBitmap(19, Y_Pos - 25, sym_trip, 50, 50, TEXT_COLOR);
  drawUnits(Y_Pos, KM);
}

void drawUMin(int Y_Pos){
  tft.drawXBitmap(19, Y_Pos - 25, sym_RPM, 50, 50, TEXT_COLOR);
  drawUnits(Y_Pos, RPM, MINUTES);
}

void drawRange(int Y_Pos){
  // Draw Petrol symbol
  tft.drawXBitmap(19, Y_Pos - 25, sym_range, 50, 50, TEXT_COLOR);
  //Draw units km
  drawUnits(Y_Pos, KM);
}

void drawDeposit(int Y_Pos){
  // Draw Petrol symbol
  tft.drawXBitmap(19, Y_Pos - 25, sym_deposit, 50, 50, TEXT_COLOR);
  //Draw units km
  drawUnits(Y_Pos, LITER);
}

void drawVolt(int Y_Pos){
  // Draw Battery symbol
  tft.drawXBitmap(19, Y_Pos - 25, sym_battery, 50, 50, TEXT_COLOR);
  //Draw units km
  drawUnits(Y_Pos, "V");
}

//void drawConsumption(int Y_Pos){
//  tft.setTextDatum(MC_DATUM);
//  tft.drawString(LITER, 220, Y_Pos, FONT4);
//}

void drawAverageConsumption(int Y_Pos){
  //Draw Average symbol
  draw_average_symbol(44, Y_Pos, 18);
  drawUnits(Y_Pos, LITER, HUNDERTKM);
}

void drawActualConsumption(int Y_Pos){
tft.drawXBitmap(19, Y_Pos - 25, sym_consumption, 50, 50, TEXT_COLOR);
//  if(velocity_actual > 5){  // wird in update_values berücksichtigt
//    drawUnits(Y_Pos, LITER, HUNDERTKM);
//  }
//  else{
//    drawUnits(Y_Pos, LITER, HOUR);
//  }
}

void drawTime(int Y_Pos){
  // Draw hours and / or minutes
  // at beginning we only need minutes, hours will be draw from update_values
  drawUnits(Y_Pos, HOUR);
  tft.drawXBitmap(19, Y_Pos - 25, sym_time, 50, 50, TEXT_COLOR);
}

void drawOutTemp(int Y_Pos){
//Draw Snowflake only if Temp under 4 degrees
//  if (temp_out <= 4){
//        tft.fillRect(Icon_Pos_Snowflake[0], Icon_Pos_Snowflake[1], 50, 50, BACK_COLOR);
//    tft.drawXBitmap(Icon_Pos_Snowflake[0], Icon_Pos_Snowflake[1], sym_snowflake, 50, 50, TEXT_COLOR);  
//  }
//  else{
//    tft.fillRect(Icon_Pos_Snowflake[0], Icon_Pos_Snowflake[1], 50, 50, BACK_COLOR);   
//        tft.drawXBitmap(Icon_Pos_Snowflake[0], Icon_Pos_Snowflake[1], sym_temp2, 50, 50, TEXT_COLOR);
//  }
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
//    tft.pushImage(90, 98, 141, 300, T4_image);
    tft.setTextDatum(MC_DATUM);;
    tft.drawString(CAR_NO, 160, 340, FONT4);
}

// need to be black to simulate switch off of display
void drawShutDown(void){
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);

//DEBUG:
  tft.setTextColor(TFT_WHITE, TFT_BLACK);   //shutdown Schriftzugimmer in weisser Schriftfarbe
  TEXT_COLOR = DAY_TEXT_COLOR;              //shutdown Timer immer in weisser Schrift
  tft.drawString("Waiting for shutdown", 20, 70, FONT2);
}

void draw_GPS_direction(int Y_Pos) {
  tft.setTextDatum(ML_DATUM);
  tft.drawString("Komp.", 14, Y_Pos, FONT4);
  if (_GPS_valid && _GPS_direction > 0) {
    dtostrf(_GPS_direction, 5, 0, TFT_String);
    strcat(TFT_String, "°");
    const char* dirs[] = {"N", "NO", "O", "SO", "S", "SW", "W", "NW"};
    int idx = (int)((_GPS_direction + 22.5) / 45.0) % 8;
    strcat(TFT_String, " ");
    strcat(TFT_String, dirs[idx]);
  } else {
    strcpy(TFT_String, "---°");
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String); 
}

void draw_GPS_speed(int Y_Pos) {
  tft.setTextDatum(ML_DATUM);
  tft.drawString("km/h", 14, Y_Pos, FONT4);
  if (_GPS_valid) {
    dtostrf(_GPS_speed, 5, 1, TFT_String);
  } else {
    strcpy(TFT_String, "---.-");
  }
  draw_value_box(Y_Pos, 134, 50, TFT_String);
}

void draw_GPS_altitude(int Y_Pos){
tft.setTextDatum(ML_DATUM); 
  tft.drawString("Hoehe", 14, Y_Pos, FONT4); 

if (_GPS_valid){
    
    int altitude_int = (int)(_GPS_altitude - 46.0f + 0.5f);  // Korrektur die gps Höhe -> zu über Meeresspiegel und runden 
    if (altitude_int < -999) altitude_int = -999;
    if (altitude_int > 9999) altitude_int = 9999;

    dtostrf(altitude_int, 4, 0, TFT_String);
    strcat(TFT_String, "m");                   
  } 
  else {
    strcpy(TFT_String, "---m");              
  }

  draw_value_box(Y_Pos, 134, 50, TFT_String);
  tft.setTextDatum(MC_DATUM);
}

void draw_compass(int cx, int cy) {
if (!_GPS_valid || !gps.course.isValid()) {
   tft.fillCircle(cx, cy, 28, BACK_COLOR);
    return;
    }
static float last_angle = 0.0f;
static bool first_draw = true;
if (!first_draw && fabs(_GPS_direction - last_angle) < 7.2f) {
  return;
}
tft.fillCircle(cx, cy, 28, BACK_COLOR);
float a = _GPS_direction * PI / 180.0f;
//float a = 90* PI / 180.0f;  //debug
  //Schaft
  int len = 13;
  int x2 = cx + (int)(sin(a) * len);
  int y2 = cy - (int)(cos(a) * len);

  for (int i = -2; i <= 2; i++) {
    int px = (int)(cos(a) * i);
    int py = (int)(sin(a) * i);
    tft.drawLine(cx + px, cy + py, x2 + px, y2 + py, TEXT_COLOR);
  }

  // Spitze
  int tip_len = 11;
  int tip_x = cx + (int)(sin(a) * (len + tip_len));
  int tip_y = cy - (int)(cos(a) * (len + tip_len));

  //Flügel
  float wing_angle = 2.6f;  // ~150° Öffnung
  int wing = 11;

  int wx1 = tip_x + (int)(sin(a + wing_angle) * wing);  
  int wy1 = tip_y - (int)(cos(a + wing_angle) * wing);   
  int wx2 = tip_x + (int)(sin(a - wing_angle) * wing);
  int wy2 = tip_y - (int)(cos(a - wing_angle) * wing);

  tft.fillTriangle(tip_x, tip_y, wx1, wy1, wx2, wy2, TEXT_COLOR);

  tft.fillCircle(cx, cy, 3, TEXT_COLOR);
  tft.drawCircle(cx, cy, 5, TEXT_COLOR);

last_angle = _GPS_direction;
first_draw = false;
}

void draw_boost_bar(int X_Pos, int Y_Pos) {
  String label = "Boost";
  String unit  = "mb";

  float value     = boost_mbar;          
  int   min_value = 900;              
  int   max_value = 2200;              
  int   lower_limit = 1750;              
  int   upper_limit = 2000;               

  int width  = 36;
  int height = 158;

  int temp = map((int)value, min_value, max_value, 0, 120);  // 0–120 Pixel

  uint16_t temp_color = TEXT_COLOR;

  if (value >= upper_limit) {
    temp_color = TFT_RED;
  }
  else if (value >= lower_limit) {
    temp_color = TFT_ORANGE;
  }

  box.setColorDepth(8);
  box.createSprite(width, height);
  box.fillSprite(BACK_COLOR);

  right_bar.pushToSprite(&box, 0, 0, TFT_BLACK);

  box.fillRect(8, height - 19 - temp, width - 20, temp, temp_color);

  box.setTextColor(TEXT_COLOR, BACK_COLOR);
  box.setTextDatum(MC_DATUM);
  box.setFreeFont(&FreeSansBold12pt7b);
  box.drawString(label, width / 2, 19 / 2, GFXFF);

  char buf[8];
  sprintf(buf, "%d", (int)value);
  String sum = String(buf) + unit;
  box.setTextDatum(MC_DATUM);
  box.drawString(sum, width / 2, height - 19 / 2, FONT2);

  box.pushSprite(X_Pos, Y_Pos, TFT_TRANSPARENT);
  box.deleteSprite();
}